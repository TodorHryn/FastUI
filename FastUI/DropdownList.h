#pragma once

#include "View.h"
#include "Geometry.h"
#include <unicode/unistr.h>

class DropdownList : public View
{
public:
	DropdownList(SizePolitics width = SizePolitics::WRAP_CONTENT, SizePolitics height = SizePolitics::MATCH_PARENT);
	virtual ~DropdownList();

	virtual void setList(const std::vector<UnicodeString>& list);
	virtual UnicodeString getSelectedItem() const;

	virtual bool onMouseEventOverlay(const MouseEvent& ev) override;
	virtual bool onMouseEvent(const MouseEvent& ev) override;

	virtual void draw(int32_t width, int32_t height) override;
	virtual void drawOverlay(int32_t width, int32_t height) override;
	virtual int32_t getMinWidth(int32_t expectedHeight) const override;
	virtual int32_t getMinHeight(int32_t expectedWidth) const override;


	int32_t m_textSize;
	Drawer::Color m_textColor;

protected:
	std::vector<UnicodeString> m_list;
	uint32_t m_selectedItem;
	
	std::vector<FastUI::Rectangle> m_listBB;
	FastUI::Rectangle m_BB;

	bool m_showItems;
	mutable int32_t m_minWidth;
};