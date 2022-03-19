#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include "Drawer.h"
#include "Events.h"

class View : public std::enable_shared_from_this<View>
{
	public:
		enum class SizePolitics {
			MAX,
			MIN
		};

		View(SizePolitics width, SizePolitics height);
		virtual ~View();

		virtual void setOnMousePress(std::function<bool(const MouseEvent &ev)> handler);
		virtual void setDrawer(std::shared_ptr<Drawer> drawer);

		virtual bool onMousePress(const MouseEvent &ev);
		virtual void onKeyboardEvent(const KeyboardEvent &ev);
		virtual void onCharInput(wchar_t ch);

		virtual void draw(int32_t width, int32_t height) = 0;

		Drawer::Color m_backgroundColor;

	protected:
		SizePolitics m_width, m_height;
		std::function<bool(const MouseEvent &ev)> m_onMousePress;
		std::shared_ptr<Drawer> m_drawer;
};