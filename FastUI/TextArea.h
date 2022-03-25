#pragma once
#include "TextField.h"

class TextArea : public TextField
{
public:
	TextArea();
	virtual ~TextArea();

	virtual void onKeyboardEvent(const KeyboardEvent &ev) override;

	virtual void draw(int32_t width, int32_t height) override;
};