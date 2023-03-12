#include "DropdownList.h"
#include <algorithm>

namespace fastui
{
	DropdownList::DropdownList(SizePolitics width, SizePolitics height)
		: View(width, height)
		, m_textSize(12)
		, m_textColor(0x00, 0x00, 0x00)
		, m_selectedItem(0)
		, m_minWidth(-1)
		, m_showItems(false)
		, m_hoveredItem(UINT32_MAX)
		, m_hoverOverlay(false)
	{
		m_hoverColor = Drawer::Color(0xF0, 0xF0, 0xF0);
	}

	DropdownList::~DropdownList()
	{
	}

	void DropdownList::setList(const std::vector<UnicodeString>& list)
	{
		m_list = list;
		m_listBB.resize(list.size());
		m_minWidth = -1;
		m_selectedItem = 0;
	}

	UnicodeString DropdownList::getSelectedItem() const
	{
		if (m_selectedItem < 0 || m_selectedItem > m_list.size())
			return UnicodeString();
		return m_list[m_selectedItem];
	}

	bool DropdownList::onMouseEventOverlay(const MouseEvent& ev)
	{
		if (m_showItems)
		{
			if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
			{
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
			}
			else if (ev.action == MouseEvent::Action::ENTER)
				m_hoverOverlay = true;
			else if (ev.action == MouseEvent::Action::LEAVE)
				m_hoverOverlay = false;
		}

		return false;
	}

	bool DropdownList::onMouseEvent(const MouseEvent& ev)
	{
		if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
		{
			m_showItems = !m_showItems;
			m_drawer->focus(shared_from_this());
		}
		View::onMouseEvent(ev);
		return true;
	}

	std::shared_ptr<View> DropdownList::getViewOverlayAtMousePos(int32_t x, int32_t y)
	{
		if (m_showItems)
		{
			for (int32_t i = 0; i < m_listBB.size(); ++i)
			{
				if (x >= m_listBB[i].x && x <= m_listBB[i].x + m_listBB[i].width
					&& y >= m_listBB[i].y && y <= m_listBB[i].y + m_listBB[i].height)
				{
					m_hoveredItem = i;
					return shared_from_this();
				}
			}
		}
		m_hoveredItem = UINT32_MAX;
		return std::shared_ptr<View>();
	}

	void DropdownList::draw(int32_t width, int32_t height)
	{
		Drawer::State state = m_drawer->state();
		m_drawer->drawRoundedRectangle(0, 0, width, height, m_mouseOver ? m_hoverColor : m_backgroundColor);

		if (getSelectedItem().size())
		{
			m_drawer->drawText(m_paddingX, m_textSize, m_textSize, m_textColor, getSelectedItem());
			m_drawer->drawText(width - m_textSize, m_textSize, m_textSize, m_textColor, "🞃");
		}

		m_drawer->setState(state);
	}

	void DropdownList::drawOverlay(int32_t width, int32_t height)
	{
		if (!m_drawer->isFocused(shared_from_this()))
			m_showItems = false;

		if (m_showItems)
		{
			Drawer::State state = m_drawer->state();

			//Draw border
			int32_t totalHeight = (m_textSize + m_paddingY * 2) * m_list.size();
			m_drawer->drawRoundedRectangle(0, 0, width, totalHeight, Drawer::Color(0xA0, 0xA0, 0xA0));
			m_drawer->drawRoundedRectangle(1, 1, width - 2, totalHeight - 1, m_backgroundColor);

			//Draw items
			for (int i = 0; i < m_list.size(); ++i)
			{
				int itemId = screenposToId(i);
				int32_t translateY = m_drawer->state().m_translate_y - state.m_translate_y;
				m_listBB[itemId] = fastui::Rectangle(0, translateY, width, m_textSize + m_paddingY * 2);
				if (m_hoverOverlay && itemId == m_hoveredItem)
					m_drawer->drawRoundedRectangle(1, 1, width - 2, m_textSize + m_paddingY * 2 - 1, m_hoverColor);
				m_drawer->drawText(m_paddingX, m_textSize, m_textSize, m_textColor, m_list[itemId]);
				m_drawer->translate(0, m_textSize + m_paddingY * 2);
			}
			m_drawer->setState(state);
			m_drawer->drawText(width - m_textSize, m_textSize, m_textSize, m_textColor, "🞃");
		}
	}

	int32_t DropdownList::getMinWidth(int32_t expectedHeight) const
	{
		if (m_minWidth < 0)
		{
			m_minWidth = 0;
			for (int32_t i = 0; i < m_list.size(); ++i)
				m_minWidth = std::max(m_minWidth, static_cast<int32_t>(m_drawer->measureText(m_textSize, m_list[i]).width + m_textSize + m_paddingX * 2));
		}

		return m_minWidth;
	}

	int32_t DropdownList::getMinHeight(int32_t expectedWidth) const
	{
		return m_textSize + m_paddingY * 2;
	}

	size_t DropdownList::screenposToId(size_t id)
	{
		size_t ret = id;
		if (id == 0)
			ret = m_selectedItem;
		else if (id <= m_selectedItem)
			ret--;
		return ret;
	}
};