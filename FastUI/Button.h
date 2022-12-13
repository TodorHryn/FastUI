#pragma once

#include "View.h"

class Button : public View
{
	public:
		Button(SizePolitics width = SizePolitics::WRAP_CONTENT, SizePolitics height = SizePolitics::MATCH_PARENT);
		virtual ~Button();

		virtual bool onMouseEvent(const MouseEvent &ev) override;

		virtual void draw(int32_t width, int32_t height) override;
		virtual int32_t getMinWidth(int32_t expectedHeight) const override;
		virtual int32_t getMinHeight(int32_t expectedWidth) const override;

		std::wstring m_text;
		int32_t m_textSize;
		Drawer::Color m_textColor;
		Drawer::Color m_pressedColor;

	protected:
		bool m_pressed;
};