#include "TextField.h"
#include <algorithm>

namespace fastui
{
	TextField::TextField(SizePolitics width, SizePolitics height)
		: View(width, height)
		, m_textSize(12)
		, m_textColor(0x00, 0x00, 0x00)
		, m_cursorPos(0)
		, m_lastCursorMoveTime(0)
		, m_editable(true)
	{
	}

	TextField::~TextField()
	{
	}

	void TextField::setOnCharInput(std::function<bool(UnicodeString::char_type ch)> onCharInput)
	{
		m_onCharInput = onCharInput;
	}

	void TextField::onKeyboardEvent(const KeyboardEvent& ev)
	{
		if (!m_editable)
			return;

		if (ev.action == KeyboardEvent::Action::PRESS || ev.action == KeyboardEvent::Action::REPEAT)
		{
			if (ev.button == KeyboardEvent::Button::BACKSPACE)
			{
				if (m_cursorPos > 0 && m_cursorPos <= m_text.size())
					m_text.erase(m_cursorPos - 1);
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

	bool TextField::onMouseEvent(const MouseEvent& ev)
	{
		if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
			m_drawer->focus(shared_from_this());
		return View::onMouseEvent(ev);
	}

	void TextField::onCharInput(UnicodeString::char_type ch)
	{
		if (!m_editable)
			return;

		if (!m_onCharInput || m_onCharInput(ch))
		{
			if (m_cursorPos >= 0 && m_cursorPos < m_text.size())
				m_text.insert(m_cursorPos, ch);
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
		m_drawer->drawRectangle(0, 0, width, height, m_backgroundColor);
		Drawer::Color borderColor = m_mouseOver || (m_drawer->isFocused(shared_from_this()) && m_editable) ? Drawer::Color(0x40, 0x40, 0x40) : Drawer::Color(0xA0, 0xA0, 0xA0);
		m_drawer->drawShadowBorder(0, 0, width, height, 2, borderColor);

		if (m_text.size())
		{
			if (m_drawer->isFocused(shared_from_this()) && (m_drawer->getTimeMs() - m_lastCursorMoveTime < 1000 || (m_drawer->getTimeMs() % 1600) > 800))
				m_drawer->drawText(
					m_paddingX,
					m_paddingY + m_textSize,
					m_textSize,
					m_textColor,
					m_text,
					m_cursorPos
				);
			else
				m_drawer->drawText(
					m_paddingX,
					m_paddingY + m_textSize,
					m_textSize,
					m_textColor,
					m_text
				);
		}

		m_drawer->setState(state);
	}

	int32_t TextField::getMinWidth(int32_t expectedHeight) const
	{
		return m_drawer->measureText(m_textSize, m_text).width + m_paddingX * 2;
	}

	int32_t TextField::getMinHeight(int32_t expectedWidth) const
	{
		return m_textSize + m_paddingY * 2;
	}
};
