#pragma once

#include "View.h"
#include "Geometry.h"
#include <unicode/unistr.h>

namespace fastui
{
	class DropdownList : public View
	{
	public:
		DropdownList(SizePolitics width = SizePolitics::WRAP_CONTENT, SizePolitics height = SizePolitics::MATCH_PARENT);
		virtual ~DropdownList();

		virtual void setList(const std::vector<UnicodeString>& list);

		virtual bool onMouseEventOverlay(const MouseEvent& ev) override;
		virtual bool onMouseEvent(const MouseEvent& ev) override;
		virtual std::shared_ptr<View> getViewOverlayAtMousePos(int32_t x, int32_t y) override;

		virtual void draw(int32_t width, int32_t height) override;
		virtual void drawOverlay(int32_t width, int32_t height) override;

		virtual UnicodeString getSelectedItem() const;
		virtual int32_t getMinWidth(int32_t expectedHeight) const override;
		virtual int32_t getMinHeight(int32_t expectedWidth) const override;

		int32_t m_textSize;
		Drawer::Color m_textColor;

	protected:
		std::vector<UnicodeString> m_list;
		uint32_t m_selectedItem;
		uint32_t m_hoveredItem;

		std::vector<Rectangle> m_listBB;
		Rectangle m_BB;

		bool m_showItems;
		bool m_hoverOverlay;
		mutable int32_t m_minWidth;
	};
};