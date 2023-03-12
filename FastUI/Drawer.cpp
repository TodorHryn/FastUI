#include "Drawer.h"
#include "View.h"
#include <fstream>
#include <GLFW\glfw3.h>
#include <glad\glad.h>
#include <algorithm>

namespace fastui
{
	Drawer::Drawer(int32_t width, int32_t height)
		: m_width(width)
		, m_height(height)
	{

	}

	Drawer::~Drawer()
	{
	}

	void Drawer::onCharInput(UnicodeString::char_type ch)
	{
		if (m_focusedView)
			m_focusedView->onCharInput(ch);
	}

	void Drawer::onKeyboardEvent(const KeyboardEvent& ev)
	{
		if (m_focusedView)
			m_focusedView->onKeyboardEvent(ev);
	}

	void Drawer::onMouseEvent(const MouseEvent& ev)
	{
		if (!m_root->onMouseEventOverlay(ev))
			m_root->onMouseEvent(ev);
	}

	void Drawer::onScroll(int32_t x, int32_t y, double xoffset, double yoffset)
	{
		m_root->onScroll(x, y, xoffset, yoffset);
	}

	void Drawer::translate(int32_t x, int32_t y)
	{
		m_state.m_translate_x += x;
		m_state.m_translate_y += y;
	}

	void Drawer::translateTo(int32_t x, int32_t y)
	{
		m_state.m_translate_x = x;
		m_state.m_translate_y = y;
	}

	void Drawer::focus(std::shared_ptr<View> view)
	{
		m_focusedView = view;
	}

	void Drawer::setRoot(std::shared_ptr<View> view)
	{
		m_root = view;
		m_root->setDrawer(shared_from_this());
	}

	void Drawer::setScissor(int32_t x, int32_t y, int32_t width, int32_t height)
	{
		m_state.m_scissorX = x + m_state.m_translate_x;
		m_state.m_scissorY = y + m_state.m_translate_y;
		m_state.m_scissorWidth = width;
		m_state.m_scissorHeight = height;
	}

	const Drawer::State& Drawer::state() const
	{
		return m_state;
	}

	void Drawer::setState(const State& state)
	{
		m_state = state;
	}

	void Drawer::setSize(int32_t width, int32_t height)
	{
		m_width = width;
		m_height = height;
	}

	int32_t Drawer::width() const
	{
		return m_width;
	}

	int32_t Drawer::height() const
	{
		return m_height;
	}

	bool Drawer::isFocused(std::shared_ptr<View> view)
	{
		return m_focusedView == view;
	}
};
