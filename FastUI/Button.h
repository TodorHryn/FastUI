#pragma once

#include "View.h"

class Button : public View
{
	public:
		Button(SizePolitics width, SizePolitics height);
		virtual ~Button();

		virtual void draw(int32_t width, int32_t height) override;

		std::wstring m_text;
		int32_t m_textSize;
		Drawer::Color m_textColor;
};