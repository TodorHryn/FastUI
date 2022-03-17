#pragma once
#include <cstdint>
#include "Drawer.h"

class View
{
	public:
		enum class SizePolitics {
			MAX,
			MIN
		};

		View(SizePolitics width, SizePolitics height);
		virtual ~View();

		virtual void setBackgroundColor(const Drawer::Color color);

		virtual void draw(Drawer &drawer, int32_t width, int32_t height) = 0;
		virtual bool onClick(int32_t x, int32_t y);

	protected:
		SizePolitics m_width, m_height;
		Drawer::Color m_backgroundColor;
};