#pragma once
#include <cstdint>

namespace FastUI
{
	struct Rectangle
	{
		Rectangle() : x(0), y(0), width(0), height(0) {}
		Rectangle(int32_t c_x, int32_t c_y, int32_t c_width, int32_t c_height) : x(c_x), y(c_y), width(c_width), height(c_height) {}

		int32_t x, y, width, height;
	};
};