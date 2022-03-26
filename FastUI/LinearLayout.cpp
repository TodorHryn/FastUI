#include "LinearLayout.h"
#include <algorithm>

float SCROLL_X_MULTIPLIER = 30;
float SCROLL_Y_MULTIPLIER = 30;

LinearLayout::LinearLayout(SizePolitics width, SizePolitics height)
	: View(width, height) 
	, m_orientation(Orientation::VERTICAL)
	, m_spacing(0)
	, m_scrollX(0)
	, m_scrollY(0)
	, m_maxScrollX(0)
	, m_maxScrollY(0)
	, m_scrollbarBackgroundColor(0x00, 0x00, 0x00, 0x40)
	, m_scrollbarColor(0x00, 0x00, 0x00, 0x80)
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

bool LinearLayout::onScroll(int32_t x, int32_t y, double xoffset, double yoffset)
{
	x += m_scrollX;
	y += m_scrollY;

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		const FastUI::Rectangle &rect = m_childrenBB[i];
		if (x >= rect.x
			&& x <= rect.x + rect.width
			&& y >= rect.y 
			&& y <= rect.y + rect.height)
		{
			if (m_children[i]->onScroll(x, y, xoffset, yoffset))
				return true;
		}
	}

	int32_t prevScrollX = m_scrollX, prevScrollY = m_scrollY;
	m_scrollX = std::max<int32_t>(0, std::min<int32_t>(m_maxScrollX, m_scrollX - xoffset * SCROLL_X_MULTIPLIER));
	m_scrollY = std::max<int32_t>(0, std::min<int32_t>(m_maxScrollY, m_scrollY - yoffset * SCROLL_Y_MULTIPLIER));
	if (m_scrollX != prevScrollX || m_scrollY != prevScrollY)
		return true;

	return View::onScroll(x, y, xoffset, yoffset);
}

bool LinearLayout::onMouseEvent(const MouseEvent &ev)
{
	MouseEvent realEv = ev;
	realEv.x += m_scrollX;
	realEv.y += m_scrollY;

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		const FastUI::Rectangle &rect = m_childrenBB[i];
		if (realEv.x >= rect.x
			&& realEv.x <= rect.x + rect.width
			&& realEv.y >= rect.y 
			&& realEv.y <= rect.y + rect.height)
		{
			MouseEvent passEv = realEv;
			passEv.x -= rect.x;
			passEv.y -= rect.y;
			if (m_children[i]->onMouseEvent(passEv))
				return true;
		}
	}

	return View::onMouseEvent(realEv);
}

void LinearLayout::draw(int32_t width, int32_t height)
{
	if (m_children.empty())
		return;

	updateBB(width, height);
	m_maxScrollX = 0;
	m_maxScrollY = 0;
	int32_t minX = m_childrenBB[0].x - m_paddingX;
	int32_t minY = m_childrenBB[0].y - m_paddingY;
	for (size_t i = 0; i < m_children.size(); ++i)
	{
		m_maxScrollX = std::max<int32_t>(m_maxScrollX, m_childrenBB[i].x + m_childrenBB[i].width + m_paddingX - minX - width);
		m_maxScrollY = std::max<int32_t>(m_maxScrollY, m_childrenBB[i].y + m_childrenBB[i].height + m_paddingY - minY - height);
	}
	m_scrollX = std::min(m_scrollX, m_maxScrollX);
	m_scrollY = std::min(m_scrollY, m_maxScrollY);

	Drawer::State state = m_drawer->state();
	m_drawer->drawRectange(0, 0, width, height, m_backgroundColor);
	for (size_t i = 0; i < m_children.size(); ++i)
	{
		m_drawer->translateTo(state.m_translate_x + m_childrenBB[i].x - m_scrollX, state.m_translate_y + m_childrenBB[i].y - m_scrollY);
		m_drawer->setScissor(0, 0, m_childrenBB[i].width, m_childrenBB[i].height);
		m_children[i]->draw(m_childrenBB[i].width, m_childrenBB[i].height);
	}
	m_drawer->setState(state);
	if (m_maxScrollY > 0)
	{
		m_drawer->drawRectange(width - 8, 0, 8, height, m_scrollbarBackgroundColor);
		float scrollbarY = static_cast<float>(m_scrollY) / (m_maxScrollY + height) * height;
		float scrollbarHeight = static_cast<float>(height) / (m_maxScrollY + height) * height;
		m_drawer->drawRectange(width - 6, scrollbarY, 4, scrollbarHeight, m_scrollbarColor);
	}
	if (m_maxScrollX > 0)
	{
		m_drawer->drawRectange(0, height - 8, width, 8, m_scrollbarBackgroundColor);
		float scrollbarX = static_cast<float>(m_scrollX) / (m_maxScrollX + width) * width;
		float scrollbarWidth = static_cast<float>(width) / (m_maxScrollX + width) * width;
		m_drawer->drawRectange(scrollbarX, height - 6, scrollbarWidth, 4, m_scrollbarColor);
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
			m_childrenBB[i].x = m_paddingX + dx;
			m_childrenBB[i].y = m_paddingY;
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
			m_childrenBB[i].x = m_paddingX;
			m_childrenBB[i].y = m_paddingY + dy;
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
