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

		LinearLayout(SizePolitics width, SizePolitics height);
		virtual ~LinearLayout();

		virtual void addChild(std::shared_ptr<View> child);
		virtual void setOrientation(Orientation orientation);
		virtual void setSpacing(int32_t spacing);

		virtual bool onMousePress(const MouseEvent &ev);

		virtual void draw(Drawer &drawer, int32_t width, int32_t height) override;

	protected:
		Orientation m_orientation;
		int32_t m_spacing;

		std::vector<std::shared_ptr<View>> m_children;
		std::vector<FastUI::Rectangle> m_childrenBB;
};