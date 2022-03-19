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

bool View::onMousePress(const MouseEvent &ev)
{
	if (m_onMousePress)
		return m_onMousePress(ev);
	else
		return false;
}
