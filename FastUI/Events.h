#pragma once

struct MouseEvent
{
	enum class Button {
		LEFT,
		RIGHT
	};

	enum class Action
	{
		PRESS,
		RELEASE
	};

	MouseEvent(Button btn, Action ac, int32_t c_x, int32_t c_y) : button(btn), action(ac), x(c_x), y(c_y) {}

	Button button;
	Action action;
	int32_t x;
	int32_t y;
};

struct KeyboardEvent
{
	enum class Button
	{
		NONE,
		BACKSPACE,
		LEFT,
		RIGHT
	};

	enum class Action
	{
		PRESS,
		RELEASE,
		REPEAT
	};

	KeyboardEvent(Button btn, Action ac) : button(btn), action(ac) {}

	Button button;
	Action action;
};