#include "LinearLayout.h"
#include <algorithm>

LinearLayout::LinearLayout(SizePolitics width, SizePolitics height)
	: View(width, height) 
	, m_orientation(Orientation::VERTICAL)
	, m_spacing(0)
{
}

LinearLayout::~LinearLayout()
{
}

void LinearLayout::addChild(std::shared_ptr<View> child)
{
	child->setDrawer(m_drawer);
	m_children.push_back(child);
	m_childrenBB.push_back(FastUI::Rectangle());
}

void LinearLayout::setOrientation(Orientation orientation)
{
	m_orientation = orientation;
}

void LinearLayout::setSpacing(int32_t spacing)
{
	m_spacing = spacing;
}

bool LinearLayout::onMouseEvent(const MouseEvent &ev)
{
	for (size_t i = 0; i < m_children.size(); ++i)
	{
		const FastUI::Rectangle &rect = m_childrenBB[i];
		if (ev.x >= rect.x
			&& ev.x <= rect.x + rect.width
			&& ev.y >= rect.y 
			&& ev.y <= rect.y + rect.height)
		{
			if (m_children[i]->onMouseEvent(ev))
				return true;
		}
	}

	return View::onMouseEvent(ev);
}

void LinearLayout::draw(int32_t width, int32_t height)
{
	if (m_children.empty())
		return;

	updateBB(width, height);

	Drawer::State state = m_drawer->state();
	m_drawer->drawRectange(0, 0, width, height, m_backgroundColor);
	for (size_t i = 0; i < m_children.size(); ++i)
	{
		m_drawer->translateTo(m_childrenBB[i].x, m_childrenBB[i].y);
		m_drawer->setScissor(0, 0, m_childrenBB[i].width, m_childrenBB[i].height);
		m_children[i]->draw(m_childrenBB[i].width, m_childrenBB[i].height);
	}
	m_drawer->setState(state);
}

int32_t LinearLayout::getMinWidth(int32_t expectedHeight) const
{
	if (m_children.empty())
		return 0;

	if (m_orientation == Orientation::VERTICAL)
	{
		updateBB(-1, expectedHeight);
		int32_t width = 0;
		for (size_t i = 0; i < m_children.size(); ++i)
			width = std::max(width, m_children[i]->getMinWidth(m_childrenBB[i].height));
		return width + m_paddingX * 2;
	}
	else
	{
		int32_t width = m_spacing * (m_children.size() - 1);
		for (size_t i = 0; i < m_children.size(); ++i)
			width += m_children[i]->getMinWidth(expectedHeight);
		return width + m_paddingX * 2;
	}
}

int32_t LinearLayout::getMinHeight(int32_t expectedWidth) const
{
	if (m_children.empty())
		return 0;

	if (m_orientation == Orientation::HORIZONTAL)
	{
		updateBB(expectedWidth, -1);
		int32_t height = 0;
		for (size_t i = 0; i < m_children.size(); ++i)
			height = std::max(height, m_children[i]->getMinHeight(m_childrenBB[i].width));
		return height + m_paddingY * 2;
	}
	else 
	{
		int32_t height = m_spacing * (m_children.size() - 1);
		for (size_t i = 0; i < m_children.size(); ++i)
			height += m_children[i]->getMinHeight(expectedWidth);
		return height + m_paddingY * 2;
	}
}

void LinearLayout::updateBB(int32_t width, int32_t height) const
{
	if (m_orientation == Orientation::HORIZONTAL) 
	{
		int32_t reservedWidth = m_spacing * (m_children.size() - 1) + m_paddingX * 2;
		size_t notFixedChildrenCount = 0;

		for (size_t i = 0; i < m_children.size(); ++i)
		{
			if (m_children[i]->getWidthPolitics() == View::SizePolitics::WRAP_CONTENT)
			{
				int32_t w = m_children[i]->getMinWidth(height - m_paddingY * 2);
				m_childrenBB[i].width = w;
				reservedWidth += w;
			}
			else
				notFixedChildrenCount++;
		}

		notFixedChildrenCount = std::max<size_t>(notFixedChildrenCount, 1); //If zero not fixed children, their width doesn't matter
		int32_t childWidth = (width - reservedWidth) / notFixedChildrenCount;
		int32_t firstChildWidth = width - reservedWidth - childWidth * (notFixedChildrenCount - 1);
		bool firstNonFixedChild = true;
		int32_t dx = 0;
		for (size_t i = 0; i < m_children.size(); ++i)
		{
			m_childrenBB[i].x = m_drawer->state().m_translate_x + m_paddingX + dx;
			m_childrenBB[i].y = m_drawer->state().m_translate_y + m_paddingY;
			m_childrenBB[i].height = height - m_paddingY * 2;

			if (m_children[i]->getWidthPolitics() == View::SizePolitics::MATCH_PARENT)
			{
				m_childrenBB[i].width = firstNonFixedChild ? firstChildWidth : childWidth;
				firstNonFixedChild = false;
			}
			
			dx += m_childrenBB[i].width + m_spacing;
		}
	}
	else
	{
		int32_t reservedHeight = m_spacing * (m_children.size() - 1) + m_paddingY * 2;
		size_t notFixedChildrenCount = 0;

		for (size_t i = 0; i < m_children.size(); ++i)
		{
			if (m_children[i]->getHeightPolitics() == View::SizePolitics::WRAP_CONTENT)
			{
				int32_t h = m_children[i]->getMinHeight(width - m_paddingX * 2);
				m_childrenBB[i].height = h;
				reservedHeight += h;
			}
			else
				notFixedChildrenCount++;
		}

		notFixedChildrenCount = std::max<size_t>(notFixedChildrenCount, 1); //If zero not fixed children, their height doesn't matter
		int32_t childHeight = (height - reservedHeight) / notFixedChildrenCount;
		int32_t firstChildHeight = height - reservedHeight - childHeight * (notFixedChildrenCount - 1);
		bool firstNonFixedChild = true;
		int32_t dy = 0;
		for (size_t i = 0; i < m_children.size(); ++i)
		{
			m_childrenBB[i].x = m_drawer->state().m_translate_x + m_paddingX;
			m_childrenBB[i].y = m_drawer->state().m_translate_y + m_paddingY + dy;
			m_childrenBB[i].width = width - m_paddingX * 2;

			if (m_children[i]->getHeightPolitics() == View::SizePolitics::MATCH_PARENT)
			{
				m_childrenBB[i].height = firstNonFixedChild ? firstChildHeight : childHeight;
				firstNonFixedChild = false;
			}
			
			dy += m_childrenBB[i].height + m_spacing;
		}
	}
}

void LinearLayout::setDrawer(std::shared_ptr<Drawer> drawer)
{
	for (auto &child : m_children)
		child->setDrawer(drawer);
	View::setDrawer(drawer);
}
