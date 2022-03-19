#include "Button.h"

Button::Button(SizePolitics width, SizePolitics height) 
	: View(width, height)
	, m_textSize(12)
	, m_textColor(Drawer::Color(0xFF, 0xFF, 0xFF))
	, m_pressedColor(m_backgroundColor)
	, m_pressed(false)
{
}

Button::~Button()
{
}

bool Button::onMouseEvent(const MouseEvent &ev)
{
	m_pressed = ev.action == MouseEvent::Action::PRESS;
	return View::onMouseEvent(ev);
}

void Button::draw(int32_t width, int32_t height)
{
	Drawer::State state = m_drawer->state();
	if (m_pressed)
		m_drawer->drawRectange(0, 0, width, height, m_pressedColor);
	else
		m_drawer->drawRectange(0, 0, width, height, m_backgroundColor);

	if (m_text.size())
	{
		auto size = m_drawer->measureText(m_textSize, m_text);
		m_drawer->drawText(width / 2 - size.first / 2, height / 2 - size.second / 2, m_textSize, m_textColor, m_text);
	}

	m_drawer->setState(state);
}
