#include "Checkbox.h"

namespace fastui
{
	Checkbox::Checkbox(SizePolitics width, SizePolitics height)
		: View(width, height)
		, m_textSize(12)
		, m_textColor(0x00, 0x00, 0x00)
		, m_checkedColor(0x50, 0x50, 0x50)
		, m_checked(false)
		, m_checkboxSize(12)
		, m_checkboxBorderSize(2)
	{
		m_hoverColor = Drawer::Color(0x30, 0x30, 0x30);
	}

	Checkbox::~Checkbox()
	{
	}

	bool Checkbox::onMouseEvent(const MouseEvent& ev)
	{
		if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
			m_checked = !m_checked;
		return View::onMouseEvent(ev);
	}

	void Checkbox::draw(int32_t width, int32_t height)
	{
		Drawer::State state = m_drawer->state();
		m_drawer->drawRectangle(0, 0, width, height, m_backgroundColor);
		m_drawer->drawRectangle(m_paddingX, (height - m_checkboxSize) / 2, m_checkboxSize, m_checkboxSize, m_mouseOver ? m_hoverColor : m_checkedColor);
		m_drawer->drawRectangle(
			m_paddingX + m_checkboxBorderSize,
			(height - m_checkboxSize) / 2 + m_checkboxBorderSize,
			m_checkboxSize - m_checkboxBorderSize * 2,
			m_checkboxSize - m_checkboxBorderSize * 2,
			m_checked ? m_checkedColor : m_backgroundColor
		);

		if (m_text.size())
		{
			Drawer::TextSize size = m_drawer->measureText(m_textSize, m_text);
			m_drawer->drawText(m_paddingX * 2 + m_checkboxSize, height / 2 + size.height() / 2, m_textSize, m_textColor, m_text);
		}

		m_drawer->setState(state);
	}

	int32_t Checkbox::getMinWidth(int32_t expectedHeight) const
	{
		return m_checkboxSize + m_drawer->measureText(m_textSize, m_text).width + m_paddingX * 3;
	}

	int32_t Checkbox::getMinHeight(int32_t expectedWidth) const
	{
		return m_textSize + m_paddingY * 2;
	}
};
