#pragma once

struct MouseEvent
{
	enum class Button{
		LEFT,
		RIGHT
	};

	enum class Action
	{
		PRESS,
		RELEASE
	};

	MouseEvent(const Button &btn, const Action &ac, int32_t c_x, int32_t c_y) : button(btn), action(ac), x(c_x), y(c_y) {}

	Button button;
	Action action;
	int32_t x;
	int32_t y;
};