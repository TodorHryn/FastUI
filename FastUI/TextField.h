#pragma once
#include "View.h"

class TextField : public View
{
public:
	TextField(SizePolitics width, SizePolitics height);
	virtual ~TextField();

	virtual void setOnCharInput(std::function<bool(wchar_t ch)> onCharInput);

	virtual bool onMouseEvent(const MouseEvent &ev) override;
	virtual void onKeyboardEvent(const KeyboardEvent &ev) override;
	virtual void onCharInput(wchar_t ch) override;

	virtual void draw(int32_t width, int32_t height) override;

	std::wstring m_text;
	int32_t m_textSize;
	Drawer::Color m_textColor;

protected:
	std::function<bool(wchar_t ch)> m_onCharInput;
};