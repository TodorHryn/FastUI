#include "Button.h"

Button::Button(SizePolitics width, SizePolitics height) 
	: View(width, height)
{
}

Button::~Button()
{
}

void Button::draw(Drawer &drawer, int32_t width, int32_t height)
{
	Drawer::State state = drawer.state();
	drawer.drawRectange(0, 0, width, height, m_backgroundColor);
	drawer.translate(width / 2, height / 2);
	drawer.drawText(0, 0, 48, Drawer::Color(0xFF, 0xFF, 0xFF), L"Hi!Ыр,^ĘŁ");
	drawer.setState(state);
}
