#pragma once

#include "View.h"

class Button : public View
{
	public:
		Button(SizePolitics width, SizePolitics height);
		virtual ~Button();

		virtual Button& setText(const std::wstring &text);
		virtual Button& setTextSize(int32_t textSize);
		virtual Button& setTextColor(const Drawer::Color &color);

		virtual void draw(Drawer &drawer, int32_t width, int32_t height) override;

		std::wstring m_text;
		int32_t m_textSize;
		Drawer::Color m_textColor;
};