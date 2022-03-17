#include "LinearLayout.h"

LinearLayout::LinearLayout(SizePolitics width, SizePolitics height)
	: View(width, height) 
	, m_orientation(Orientation::VERTICAL)
{
}

LinearLayout::~LinearLayout()
{
}

void LinearLayout::addChild(std::shared_ptr<View> child)
{
	m_children.push_back(child);
}

void LinearLayout::setOrientation(Orientation orientation)
{
	m_orientation = orientation;
}

void LinearLayout::setSpacing(int32_t spacing)
{
	m_spacing = spacing;
}

void LinearLayout::draw(Drawer &drawer, int32_t width, int32_t height)
{
	if (m_children.empty())
		return;

	if (m_orientation == Orientation::HORIZONTAL) 
	{
		int32_t childWidth = (width - m_spacing * (m_children.size() - 1)) / m_children.size();
		Drawer::State state = drawer.state();

		m_children[0]->draw(drawer, width - (childWidth + m_spacing) * (m_children.size() - 1), height);
		drawer.translate(width - (childWidth + m_spacing) * (m_children.size() - 1) + m_spacing, 0);
		for (size_t i = 1; i < m_children.size(); ++i)
		{
			m_children[i]->draw(drawer, childWidth, height);
			drawer.translate(childWidth + m_spacing, 0);
		}

		drawer.setState(state);
	}
	else
	{
		int32_t childHeight = (height - m_spacing * (m_children.size() - 1)) / m_children.size();
		Drawer::State state = drawer.state();

		m_children[0]->draw(drawer, width, height - (childHeight + m_spacing) * (m_children.size() - 1));
		drawer.translate(0, height - (childHeight + m_spacing) * (m_children.size() - 1) + m_spacing);
		for (size_t i = 1; i < m_children.size(); ++i)
		{
			m_children[i]->draw(drawer, width, childHeight);
			drawer.translate(0, childHeight + m_spacing);
		}

		drawer.setState(state);
	}
}
