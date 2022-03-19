#include "View.h"

View::View(SizePolitics width, SizePolitics height)
{
}

View::~View()
{
}

void View::setOnMouseEvent(std::function<bool(const MouseEvent&ev)> handler)
{
	m_onMouseEvent = handler;
}
void View::setDrawer(std::shared_ptr<Drawer> drawer)
{
	m_drawer = drawer;
}

bool View::onMouseEvent(const MouseEvent &ev)
{
	if (m_onMouseEvent)
		return m_onMouseEvent(ev);
	else
		return false;
}

void View::onKeyboardEvent(const KeyboardEvent &ev)
{
	
}

void View::onCharInput(wchar_t ch)
{
}
