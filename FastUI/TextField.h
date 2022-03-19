#pragma once
#include "View.h"

class TextField : public View
{
public:
	TextField(SizePolitics width, SizePolitics height);
	virtual ~TextField();

	virtual bool onMousePress(const MouseEvent &ev) override;
	virtual void onKeyboardEvent(const KeyboardEvent &ev) override;
	virtual void onCharInput(wchar_t ch) override;

	virtual void draw(int32_t width, int32_t height) override;

	std::wstring m_text;
	int32_t m_textSize;
	Drawer::Color m_textColor;
};