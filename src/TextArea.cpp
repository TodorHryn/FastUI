#include "TextArea.h"

namespace fastui
{
	TextArea::TextArea(SizePolitics width, SizePolitics height)
		: TextField(width, height)
	{
	}

	TextArea::~TextArea()
	{
	}

	void TextArea::onKeyboardEvent(const KeyboardEvent& ev)
	{
		if (!m_editable)
			return;

		TextField::onKeyboardEvent(ev);
		if (ev.action == KeyboardEvent::Action::PRESS || ev.action == KeyboardEvent::Action::REPEAT)
		{
			if (ev.button == KeyboardEvent::Button::ENTER)
			{
				onCharInput(L'\n');
			}
		}
	}

	int32_t TextArea::getMinWidth(int32_t expectedHeight) const
	{
		return 0; //TODO: calculate when width and height are WRAP_CONTENT
	}

	int32_t TextArea::getMinHeight(int32_t expectedWidth) const
	{
		return m_drawer->measureText(expectedWidth - m_paddingX * 2, m_textSize, m_text).height() + m_paddingY * 2;
	}

	void TextArea::draw(int32_t width, int32_t height)
	{
		Drawer::State state = m_drawer->state();
		m_drawer->drawRectangle(0, 0, width, height, m_backgroundColor);
		Drawer::Color borderColor = m_mouseOver || (m_drawer->isFocused(shared_from_this()) && m_editable) ? Drawer::Color(0x40, 0x40, 0x40) : Drawer::Color(0xA0, 0xA0, 0xA0);
		m_drawer->drawShadowBorder(0, 0, width, height, 2, borderColor);

		if (m_text.size())
		{
			auto size = m_drawer->measureText(m_textSize, m_text);

			if (m_drawer->isFocused(shared_from_this()) && (m_drawer->getTimeMs() - m_lastCursorMoveTime < 1000 || (m_drawer->getTimeMs() % 1600) > 800))
				m_drawer->drawText(
					m_paddingX,
					m_paddingY + m_textSize,
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
					m_paddingY + m_textSize,
					width - m_paddingX * 2,
					height - m_paddingY * 2,
					m_textSize,
					m_textColor,
					m_text
				);
		}

		m_drawer->setState(state);
	}
};
