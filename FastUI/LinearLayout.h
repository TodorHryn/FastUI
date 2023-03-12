#pragma once
#include "View.h"
#include "Geometry.h"
#include <vector>
#include <memory>

namespace fastui
{
	class LinearLayout : public View
	{
	public:
		enum class Orientation {
			HORIZONTAL,
			VERTICAL
		};

		LinearLayout(SizePolitics width = SizePolitics::MATCH_PARENT, SizePolitics height = SizePolitics::MATCH_PARENT);
		virtual ~LinearLayout();

		virtual void addChild(std::shared_ptr<View> child);
		virtual void setOrientation(Orientation orientation);
		virtual void setSpacing(int32_t spacing);

		virtual bool onScroll(int32_t x, int32_t y, double xoffset, double yoffset) override;
		virtual bool onMouseEvent(const MouseEvent& ev) override;
		virtual bool onMouseEventOverlay(const MouseEvent& ev) override;

		virtual void draw(int32_t width, int32_t height) override;
		virtual void drawOverlay(int32_t width, int32_t height) override;

		virtual int32_t getMinWidth(int32_t expectedHeight) const override;
		virtual int32_t getMinHeight(int32_t expectedWidth) const override;

		Drawer::Color m_scrollbarColor;

	protected:
		void updateBB(int32_t width, int32_t height) const;
		virtual void setDrawer(std::shared_ptr<Drawer> drawer);

		bool m_hasVerticalScrollbar, m_hasHorizontalScrollbar;
		Orientation m_orientation;
		int32_t m_spacing;
		int32_t m_scrollX, m_scrollY;
		int32_t m_maxScrollX, m_maxScrollY;

		std::vector<std::shared_ptr<View>> m_children;
		mutable std::vector<fastui::Rectangle> m_childrenBB;
	};
};
