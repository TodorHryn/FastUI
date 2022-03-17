#include "Drawer.h"
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

Drawer::Drawer(int32_t width, int32_t height)
	: m_width(width)
	, m_height(height)
{
	m_buffer = new unsigned char[width * height * 4]();
}

Drawer::~Drawer()
{
}

void Drawer::clear()
{
	for (int32_t x = 0; x < m_width; ++x)
	{
		for (int32_t y = 0; y < m_height; ++y)
		{
			m_buffer[y * m_width * 4 + x * 4] = 0x00;
			m_buffer[y * m_width * 4 + x * 4 + 1] = 0x00;
			m_buffer[y * m_width * 4 + x * 4 + 2] = 0x00;
			m_buffer[y * m_width * 4 + x * 4 + 3] = 0xFF;
		}
	}
	m_state = State();
}

void Drawer::translate(int32_t x, int32_t y)
{
	m_state.m_translate_x += x;
	m_state.m_translate_y += y;
}

void Drawer::drawRectange(int32_t x, int32_t y, int32_t width, int32_t height, Color color)
{
	x += m_state.m_translate_x;
	y += m_state.m_translate_y;

	for (int32_t i = 0; i < width && x + i < m_width; ++i)
	{
		for (int32_t j = 0; j < height && y + j < m_height; ++j)
		{
			uint8_t &r = m_buffer[(x + i) * 4 + (y + j) * m_width * 4];
			uint8_t &g = m_buffer[(x + i) * 4 + (y + j) * m_width * 4 + 1];
			uint8_t &b = m_buffer[(x + i) * 4 + (y + j) * m_width * 4 + 2];
			uint8_t &a = m_buffer[(x + i) * 4 + (y + j) * m_width * 4 + 3];

			uint8_t a0 = a + color.A * (1 - a);
			if (a0 > 0)
			{
				r = (r * a + color.R * color.A * (1-a)) / a0;
				g = (g * a + color.G * color.A * (1-a)) / a0;
				b = (b * a + color.B * color.A * (1-a)) / a0;
				a = a0;
			}
		}
	}
}

void Drawer::drawText(int32_t x, int32_t y, int32_t size, Color color, const std::wstring &text)
{
	
}

Drawer::State Drawer::state() const
{
	return m_state;
}

void Drawer::setState(const State &state)
{
	m_state = state;
}

unsigned char* Drawer::get()
{
	return m_buffer;
}
