#include "TextArea.h"

TextArea::TextArea(SizePolitics width, SizePolitics height)
	: TextField(width, height)
{
}

TextArea::~TextArea()
{
}

void TextArea::onKeyboardEvent(const KeyboardEvent &ev)
{
	TextField::onKeyboardEvent(ev);
	if (ev.action == KeyboardEvent::Action::PRESS || ev.action == KeyboardEvent::Action::REPEAT)
	{
		if (ev.button == KeyboardEvent::Button::ENTER)
		{
			onCharInput(L'\n');
		}
	}
}

int32_t TextArea::getMinHeight(int32_t expectedWidth) const
{
	return m_drawer->measureText(expectedWidth, m_textSize, m_text).second + m_paddingY * 2;
}

void TextArea::draw(int32_t width, int32_t height)
{
	Drawer::State state = m_drawer->state();
	m_drawer->drawRectange(0, 0, width, height, m_backgroundColor);

	if (m_text.size())
	{
		auto size = m_drawer->measureText(m_textSize, m_text);

		if (m_drawer->isFocused(shared_from_this()) && (m_drawer->getTimeMs() - m_lastCursorMoveTime < 1000 || (m_drawer->getTimeMs() % 1600) > 800))
			m_drawer->drawText(
				m_paddingX,
				m_paddingY,
				width - m_paddingX * 2,
				height - m_paddingY * 2,
				m_textSize,
				m_textColor,
				m_text,
				m_cursorPos
			);
		else
			m_drawer->drawText(
				m_paddingX,
				m_paddingY,
				width - m_paddingX * 2,
				height - m_paddingY * 2,
				m_textSize,
				m_textColor,
				m_text
			);
	}

	m_drawer->setState(state);
}
