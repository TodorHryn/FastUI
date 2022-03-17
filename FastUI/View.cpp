#include "View.h"

View::View(SizePolitics width, SizePolitics height)
{
}

View::~View()
{
}

void View::setBackgroundColor(const Drawer::Color color)
{
	m_backgroundColor = color;
}

bool View::onClick(int32_t x, int32_t y)
{
	return false;
}
