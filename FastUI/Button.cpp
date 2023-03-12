#include "Button.h"

namespace fastui
{
	Button::Button(SizePolitics width, SizePolitics height)
		: View(width, height)
		, m_textSize(12)
		, m_textColor(0x00, 0x00, 0x00)
		, m_pressedColor(0xF0, 0xF0, 0xF0)
		, m_pressed(false)
	{
	}

	Button::~Button()
	{
	}

	bool Button::onMouseEvent(const MouseEvent& ev)
	{
		if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
			m_pressed = true;
		else if (ev.action == MouseEvent::Action::RELEASE || ev.action == MouseEvent::Action::LEAVE)
			m_pressed = false;
		return View::onMouseEvent(ev);
	}

	void Button::draw(int32_t width, int32_t height)
	{
		Drawer::State state = m_drawer->state();
		if (m_pressed)
		{
			m_drawer->drawRoundedRectangle(0, 0, width, height, Drawer::Color(0xDD, 0xDD, 0xDD));
			m_drawer->drawRoundedRectangle(1, 1, width - 2, height - 1, m_pressedColor);
		}
		else
		{
			m_drawer->drawRoundedRectangle(0, 0, width, height, Drawer::Color(0xD0, 0xD0, 0xD0));
			m_drawer->drawRoundedRectangle(1, 1, width - 2, height - 2, m_mouseOver ? m_hoverColor : m_backgroundColor);
		}

		if (m_text.size())
		{
			auto size = m_drawer->measureText(m_textSize, m_text);
			m_drawer->drawText(width / 2 - size.first / 2, 0, m_textSize, m_textColor, m_text);
		}

		m_drawer->setState(state);
	}

	int32_t Button::getMinWidth(int32_t expectedHeight) const
	{
		return m_drawer->measureText(m_textSize, m_text).first + m_paddingX * 2;
	}

	int32_t Button::getMinHeight(int32_t expectedWidth) const
	{
		return m_textSize + m_paddingY * 2;
	}
};
