#include "View.h"

namespace fastui
{
	View::View(SizePolitics width, SizePolitics height)
		: m_width(width)
		, m_height(height)
		, m_paddingX(0)
		, m_paddingY(0)
		, m_backgroundColor(0xFF, 0xFF, 0xFF, 0x00)
		, m_hoverColor(0xF8, 0xF8, 0xF8)
		, m_mouseOver(false)
	{
	}

	View::~View()
	{
	}

	void View::setOnMouseEvent(std::function<bool(const MouseEvent& ev)> handler)
	{
		m_onMouseEvent = handler;
	}
	void View::setDrawer(std::shared_ptr<Drawer> drawer)
	{
		m_drawer = drawer;
	}

	void View::setSizePolitics(SizePolitics width, SizePolitics height)
	{
		m_width = width;
		m_height = height;
	}

	bool View::onMouseEvent(const MouseEvent& ev)
	{
		if (ev.action == MouseEvent::Action::ENTER)
			m_mouseOver = true;
		else if (ev.action == MouseEvent::Action::LEAVE)
			m_mouseOver = false;

		if (m_onMouseEvent)
			return m_onMouseEvent(ev);
		else
			return false;
	}

	void View::onKeyboardEvent(const KeyboardEvent& ev)
	{

	}

	void View::onCharInput(UnicodeString::char_type ch)
	{
	}

	bool View::onScroll(int32_t x, int32_t y, double xoffset, double yoffset)
	{
		return false;
	}

	bool View::onMouseMove(int32_t x, int32_t y)
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

	std::shared_ptr<View> View::getViewAtMousePos(int32_t x, int32_t y)
	{
		return shared_from_this();
	}
	std::shared_ptr<View> View::getViewOverlayAtMousePos(int32_t x, int32_t y)
	{
		return std::shared_ptr<View>();
	}
};
