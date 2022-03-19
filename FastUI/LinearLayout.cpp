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

	if (m_orientation == Orientation::HORIZONTAL) 
	{
		int32_t childWidth = (width - m_spacing * (m_children.size() - 1)) / m_children.size();
		Drawer::State state = m_drawer->state();

		m_drawer->drawRectange(0, 0, width, height, m_backgroundColor);
		m_drawer->setScissor(0, 0, childWidth, height);
		int32_t firstChildWidth = width - (childWidth + m_spacing) * (m_children.size() - 1);
		m_children[0]->draw(firstChildWidth, height);
		m_childrenBB[0] = FastUI::Rectangle(m_drawer->state().m_translate_x, m_drawer->state().m_translate_y, firstChildWidth, height);
		m_drawer->translate(width - (childWidth + m_spacing) * (m_children.size() - 1) + m_spacing, 0);
		for (size_t i = 1; i < m_children.size(); ++i)
		{
			m_drawer->setScissor(0, 0, childWidth, height);
			m_children[i]->draw(childWidth, height);
			m_childrenBB[i] = FastUI::Rectangle(m_drawer->state().m_translate_x, m_drawer->state().m_translate_y, childWidth, height);
			m_drawer->translate(childWidth + m_spacing, 0);
		}

		m_drawer->setState(state);
	}
	else
	{
		int32_t childHeight = (height - m_spacing * (m_children.size() - 1)) / m_children.size();
		Drawer::State state = m_drawer->state();

		m_drawer->drawRectange(0, 0, width, height, m_backgroundColor);
		m_drawer->setScissor(0, 0, width, childHeight);
		int32_t firstChildHeight = height - (childHeight + m_spacing) * (m_children.size() - 1);
		m_children.back()->draw(width, firstChildHeight);
		m_childrenBB.back() = FastUI::Rectangle(m_drawer->state().m_translate_x, m_drawer->state().m_translate_y, width, firstChildHeight);
		m_drawer->translate(0, height - (childHeight + m_spacing) * (m_children.size() - 1) + m_spacing);
		for (int i = m_children.size() - 2; i >= 0; --i)
		{
			m_drawer->setScissor(0, 0, width, childHeight);
			m_children[i]->draw(width, childHeight);
			m_childrenBB[i] = FastUI::Rectangle(m_drawer->state().m_translate_x, m_drawer->state().m_translate_y, width, childHeight);
			m_drawer->translate(0, childHeight + m_spacing);
		}

		m_drawer->setState(state);
	}
}

void LinearLayout::setDrawer(std::shared_ptr<Drawer> drawer)
{
	for (auto &child : m_children)
		child->setDrawer(drawer);
	View::setDrawer(drawer);
}
