#pragma once
#include "View.h"

class TextField : public View
{
public:
	TextField(SizePolitics width = SizePolitics::MATCH_PARENT, SizePolitics height = SizePolitics::WRAP_CONTENT);
	virtual ~TextField();

	virtual void setOnCharInput(std::function<bool(wchar_t ch)> onCharInput);

	virtual bool onMouseEvent(const MouseEvent &ev) override;
	virtual void onKeyboardEvent(const KeyboardEvent &ev) override;
	virtual void onCharInput(wchar_t ch) override;

	virtual void draw(int32_t width, int32_t height) override;
	virtual int32_t getMinWidth(int32_t expectedHeight) const;
	virtual int32_t getMinHeight(int32_t expectedWidth) const;

	std::wstring m_text;
	int32_t m_textSize;
	int32_t m_cursorPos;
	Drawer::Color m_textColor;
	bool m_editable;

protected:
	int32_t m_lastCursorMoveTime;
	std::function<bool(wchar_t ch)> m_onCharInput;
};