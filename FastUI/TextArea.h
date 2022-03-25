#pragma once
#include "TextField.h"

class TextArea : public TextField
{
public:
	TextArea(SizePolitics width = SizePolitics::MATCH_PARENT, SizePolitics height = SizePolitics::MATCH_PARENT);
	virtual ~TextArea();

	virtual void onKeyboardEvent(const KeyboardEvent &ev) override;

	virtual void draw(int32_t width, int32_t height) override;
	virtual int32_t getMinHeight(int32_t expectedWidth) const override;
};