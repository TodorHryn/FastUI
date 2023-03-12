#include "DropdownList.h"
#include <algorithm>

DropdownList::DropdownList(SizePolitics width, SizePolitics height)
	: View(width, height)
	, m_textSize(12)
	, m_textColor(0x00, 0x00, 0x00)
	, m_selectedItem(0)
	, m_minWidth(-1)
	, m_showItems(false)
{
}

DropdownList::~DropdownList()
{
}

void DropdownList::setList(const std::vector<std::wstring>& list)
{
	m_list = list;
	m_listBB.resize(list.size());
	m_minWidth = -1;
	m_selectedItem = 0;
}

std::wstring DropdownList::getSelectedItem() const
{
	if (m_selectedItem < 0 || m_selectedItem > m_list.size())
		return std::wstring();
	return m_list[m_selectedItem];
}

bool DropdownList::onMouseEventOverlay(const MouseEvent& ev)
{
	if (!m_showItems)
		return false;

	for (int32_t i = 0; i < m_listBB.size(); ++i)
	{
		if (ev.x >= m_listBB[i].x && ev.x <= m_listBB[i].x + m_listBB[i].width
			&& ev.y >= m_listBB[i].y && ev.y <= m_listBB[i].y + m_listBB[i].height
			&& i != m_selectedItem)
		{
			m_selectedItem = i;
			m_showItems = false;
			return true;
		}
	}

	return false;
}

bool DropdownList::onMouseEvent(const MouseEvent& ev)
{
	if (ev.action == MouseEvent::Action::PRESS)
		m_showItems = !m_showItems;
	return true;
}

void DropdownList::draw(int32_t width, int32_t height)
{
	Drawer::State state = m_drawer->state();
	m_drawer->drawRectange(0, 0, width, height, m_backgroundColor);

	if (getSelectedItem().size())
	{
		m_drawer->drawText(m_paddingX, height / 2 - m_textSize / 2, m_textSize, m_textColor, getSelectedItem());
	}

	m_drawer->setState(state);
}

void DropdownList::drawOverlay(int32_t width, int32_t height)
{
	if (m_showItems)
	{
		Drawer::State state = m_drawer->state();
		m_drawer->translate(0, height);
		for (int i = 0; i < m_list.size(); ++i)
		{
			if (m_selectedItem == i)
				continue;
			auto size = m_drawer->measureText(m_textSize, m_list[i]);
			int32_t translateY = m_drawer->state().m_translate_y - state.m_translate_y;
			m_listBB[i] = FastUI::Rectangle(0, translateY, width, size.second + m_paddingY * 2);
			m_drawer->drawRectange(0, 0, width, size.second + m_paddingY * 2, m_backgroundColor);
			m_drawer->drawText(m_paddingX, size.second / 2 - m_textSize / 2, m_textSize, m_textColor, m_list[i]);
			m_drawer->translate(0, size.second + m_paddingY * 2);
		}

		m_drawer->setState(state);
	}
}

int32_t DropdownList::getMinWidth(int32_t expectedHeight) const
{
	if (m_minWidth < 0)
	{
		m_minWidth = 0;
		for (int32_t i = 0; i < m_list.size(); ++i)
			m_minWidth = std::max(m_minWidth, static_cast<int32_t>(m_drawer->measureText(m_textSize, m_list[i]).first + m_paddingX * 2));
	}

	return m_minWidth;
}

int32_t DropdownList::getMinHeight(int32_t expectedWidth) const
{
	return m_textSize + m_paddingY * 2;
}