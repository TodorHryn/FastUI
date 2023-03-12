#include "Checkbox.h"

Checkbox::Checkbox(SizePolitics width, SizePolitics height)
	: View(width, height)
	, m_textSize(12)
	, m_textColor(0x00, 0x00, 0x00)
	, m_checkedColor(0x44, 0x44, 0x44)
	, m_notCheckedColor(0xDD, 0xDD, 0xDD)
	, m_checked(false)
	, m_checkboxSize(12)
{
}

Checkbox::~Checkbox()
{
}

bool Checkbox::onMouseEvent(const MouseEvent& ev)
{
	if (ev.action == MouseEvent::Action::PRESS)
		m_checked = !m_checked;
	return View::onMouseEvent(ev);
}

void Checkbox::draw(int32_t width, int32_t height)
{
	Drawer::State state = m_drawer->state();
	m_drawer->drawRectange(0, 0, width, height, m_backgroundColor);
	m_drawer->drawRectange(m_paddingX, (height - m_checkboxSize) / 2, m_checkboxSize, m_checkboxSize, m_checked ? m_checkedColor : m_notCheckedColor);

	if (m_text.size())
	{
		auto size = m_drawer->measureText(m_textSize, m_text);
		m_drawer->drawText(m_paddingX * 2 + m_checkboxSize, height / 2 - m_textSize / 2, m_textSize, m_textColor, m_text);
	}

	m_drawer->setState(state);
}

int32_t Checkbox::getMinWidth(int32_t expectedHeight) const
{
	return m_checkboxSize + m_drawer->measureText(m_textSize, m_text).first + m_paddingX * 3;
}

int32_t Checkbox::getMinHeight(int32_t expectedWidth) const
{
	return m_textSize + m_paddingY * 2;
}
