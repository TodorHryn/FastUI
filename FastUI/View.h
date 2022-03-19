#pragma once
#include <cstdint>
#include <functional>
#include "Drawer.h"
#include "Events.h"

class View
{
	public:
		enum class SizePolitics {
			MAX,
			MIN
		};

		View(SizePolitics width, SizePolitics height);
		virtual ~View();

		virtual void setOnMousePress(std::function<bool(const MouseEvent &ev)> handler);

		virtual bool onMousePress(const MouseEvent &ev);

		virtual void draw(Drawer &drawer, int32_t width, int32_t height) = 0;

		Drawer::Color m_backgroundColor;

	protected:
		SizePolitics m_width, m_height;
		std::function<bool(const MouseEvent &ev)> m_onMousePress;
};