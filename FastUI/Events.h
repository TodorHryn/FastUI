#pragma once

namespace fastui
{
	struct MouseEvent
	{
		enum class Button {
			NONE,
			LEFT,
			RIGHT
		};

		enum class Action
		{
			NONE,
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
			RIGHT,
			ENTER
		};

		enum class Action
		{
			NONE,
			PRESS,
			RELEASE,
			REPEAT
		};

		KeyboardEvent(Button btn, Action ac) : button(btn), action(ac) {}

		Button button;
		Action action;
	};
};