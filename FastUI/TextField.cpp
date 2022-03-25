#include "TextField.h"
#include <algorithm>

TextField::TextField(SizePolitics width, SizePolitics height)
	: View(width, height)
	, m_textSize(12)
	, m_textColor(Drawer::Color(0xFF, 0xFF, 0xFF))
	, m_cursorPos(0)
	, m_lastCursorMoveTime(0)
	, m_editable(true)
{
}

TextField::~TextField()
{
}

void TextField::setOnCharInput(std::function<bool(wchar_t ch)> onCharInput)
{
	m_onCharInput = onCharInput;
}

bool TextField::onMouseEvent(const MouseEvent &ev)
{
	if (!m_editable)
		return false;

	if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
		m_drawer->focus(shared_from_this());
	return View::onMouseEvent(ev);
}

void TextField::onKeyboardEvent(const KeyboardEvent &ev)
{
	if (!m_editable)
		return;

	if (ev.action == KeyboardEvent::Action::PRESS || ev.action == KeyboardEvent::Action::REPEAT)
	{
		if (ev.button == KeyboardEvent::Button::BACKSPACE)
		{
			if (m_cursorPos > 0 && m_cursorPos <= m_text.size())
				m_text.erase(m_text.begin() + m_cursorPos - 1);
			else if (m_cursorPos < 0)
				m_text.pop_back();
			if (m_cursorPos > 0)
				m_cursorPos--;
			m_lastCursorMoveTime = m_drawer->getTimeMs();
		}
		else if (ev.button == KeyboardEvent::Button::LEFT)
		{
			if (m_cursorPos > 0)
				m_cursorPos--;
			m_lastCursorMoveTime = m_drawer->getTimeMs();
		}
		else if (ev.button == KeyboardEvent::Button::RIGHT)
		{
			if (m_cursorPos < m_text.size())
				m_cursorPos++;
			m_lastCursorMoveTime = m_drawer->getTimeMs();
		}
	}
}

void TextField::onCharInput(wchar_t ch)
{
	if (!m_editable)
		return;

	if (!m_onCharInput || m_onCharInput(ch))
	{
		if (m_cursorPos >= 0 && m_cursorPos < m_text.size())
			m_text.insert(m_text.begin() + m_cursorPos, ch);
		else
			m_text += ch;

		if (m_cursorPos >= 0)
			m_cursorPos++;
		m_lastCursorMoveTime = m_drawer->getTimeMs();
	}
}

void TextField::draw(int32_t width, int32_t height)
{
	Drawer::State state = m_drawer->state();
	m_drawer->drawRectange(0, 0, width, height, m_backgroundColor);

	if (m_text.size())
	{
		if (m_drawer->isFocused(shared_from_this()) && (m_drawer->getTimeMs() - m_lastCursorMoveTime < 1000 || (m_drawer->getTimeMs() % 1600) > 800))
			m_drawer->drawText(
				m_paddingX,
				m_paddingY,
				m_textSize,
				m_textColor,
				m_text,
				m_cursorPos
			);
		else
			m_drawer->drawText(
				m_paddingX,
				m_paddingY,
				m_textSize,
				m_textColor,
				m_text
			);
	}

	m_drawer->setState(state);
}

int32_t TextField::getMinWidth(int32_t expectedHeight) const
{
	return m_drawer->measureText(m_textSize, m_text).first + m_paddingX * 2;
}

int32_t TextField::getMinHeight(int32_t expectedWidth) const
{
	return m_textSize + m_paddingY * 2;
}
