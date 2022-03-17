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
	drawer.drawRectange(0, 0, width, height, m_backgroundColor);
}
