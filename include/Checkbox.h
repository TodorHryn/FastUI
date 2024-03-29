#pragma once

#include "View.h"
#include "UnicodeString.h"

namespace fastui
{
	class Checkbox : public View
	{
	public:
		Checkbox(SizePolitics width = SizePolitics::WRAP_CONTENT, SizePolitics height = SizePolitics::WRAP_CONTENT);
		virtual ~Checkbox();

		virtual bool onMouseEvent(const MouseEvent& ev) override;

		virtual void draw(int32_t width, int32_t height) override;

		virtual int32_t getMinWidth(int32_t expectedHeight) const override;
		virtual int32_t getMinHeight(int32_t expectedWidth) const override;

		UnicodeString m_text;
		int32_t m_textSize;
		int32_t m_checkboxSize;
		int32_t m_checkboxBorderSize;
		Drawer::Color m_textColor;
		Drawer::Color m_checkedColor;

	protected:
		bool m_checked;
	};
};