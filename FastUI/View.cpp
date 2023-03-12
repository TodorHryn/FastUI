#include "View.h"

View::View(SizePolitics width, SizePolitics height)
	: m_width(width)
	, m_height(height)
	, m_paddingX(0)
	, m_paddingY(0)
	, m_backgroundColor(0xFF, 0xFF, 0xFF, 0x00)
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

void View::onCharInput(UChar32 ch)
{
}

bool View::onScroll(int32_t x, int32_t y, double xoffset, double yoffset)
{
	return false;
}

bool View::onMouseEventOverlay(const MouseEvent& ev)
{
	return false;
}

void View::drawOverlay(int32_t width, int32_t height)
{
}

int32_t View::getMinWidth(int32_t expectedHeight) const
{
	return 100;
}

int32_t View::getMinHeight(int32_t expectedWidth) const
{
	return 100;
}

const View::SizePolitics& View::getWidthPolitics() const
{
	return m_width;
}

const View::SizePolitics& View::getHeightPolitics() const
{
	return m_height;
}
