#pragma once
#include "View.h"
#include "Geometry.h"
#include <vector>
#include <memory>

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

		virtual bool onMouseEvent(const MouseEvent &ev) override;

		virtual void draw(int32_t width, int32_t height) override;
		virtual int32_t getMinWidth(int32_t expectedHeight) const override;
		virtual int32_t getMinHeight(int32_t expectedWidth) const override;

	protected:
		void updateBB(int32_t width, int32_t height) const;
		virtual void setDrawer(std::shared_ptr<Drawer> drawer);

		Orientation m_orientation;
		int32_t m_spacing;

		std::vector<std::shared_ptr<View>> m_children;
		mutable std::vector<FastUI::Rectangle> m_childrenBB;
};