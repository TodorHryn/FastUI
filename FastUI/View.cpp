#include "View.h"

View::View(SizePolitics width, SizePolitics height)
{
}

View::~View()
{
}

void View::setOnMousePress(std::function<bool(const MouseEvent&ev)> handler)
{
	m_onMousePress = handler;
}
void View::setDrawer(std::shared_ptr<Drawer> drawer)
{
	m_drawer = drawer;
}

bool View::onMousePress(const MouseEvent &ev)
{
	if (m_onMousePress)
		return m_onMousePress(ev);
	else
		return false;
}

void View::onKeyboardEvent(const KeyboardEvent &ev)
{
	
}

void View::onCharInput(wchar_t ch)
{
}
