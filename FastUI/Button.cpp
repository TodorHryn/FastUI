#include "Button.h"

Button::Button(SizePolitics width, SizePolitics height) 
	: View(width, height)
	, m_textSize(12)
	, m_textColor(Drawer::Color(0xFF, 0xFF, 0xFF))
{
}

Button::~Button()
{
}

Button& Button::setText(const std::wstring &text)
{
	m_text = text;
	return *this;
}

Button& Button::setTextSize(int32_t textSize)
{
	m_textSize = textSize;
	return *this;
}

Button& Button::setTextColor(const Drawer::Color &color)
{
	m_textColor = color;
	return *this;
}

void Button::draw(Drawer &drawer, int32_t width, int32_t height)
{
	Drawer::State state = drawer.state();
	drawer.drawRectange(0, 0, width, height, m_backgroundColor);

	if (m_text.size())
	{
		auto size = drawer.measureText(m_textSize, m_text);
		drawer.drawText(width / 2 - size.first / 2, height / 2 - size.second / 2, m_textSize, m_textColor, m_text);
	}

	drawer.setState(state);
}
