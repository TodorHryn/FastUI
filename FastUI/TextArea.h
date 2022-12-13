#pragma once
#include "TextField.h"

class TextArea : public TextField
{
public:
	TextArea(SizePolitics width = SizePolitics::MATCH_PARENT, SizePolitics height = SizePolitics::WRAP_CONTENT);
	virtual ~TextArea();

	virtual void onKeyboardEvent(const KeyboardEvent &ev) override;

	virtual void draw(int32_t width, int32_t height) override;

	virtual int32_t getMinWidth(int32_t expectedHeight) const;
	virtual int32_t getMinHeight(int32_t expectedWidth) const override;
};