#include "Drawer.h"
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <GLFW\glfw3.h>
#include <glad\glad.h>

Drawer::Drawer(int32_t width, int32_t height)
	: m_width(width)
	, m_height(height)
	, m_rectShader("shader")
	, m_charShader("char")
	, m_font(L"OpenSans-Regular.ttf")
{
	float rect[] = {
		-1.0f, -1.0f, 0.0f,		
		1.0f, -1.0f, 0.0f,	
		1.0f, 1.0f, 0.0f,		

		-1.0f, -1.0f, 0.0f,	
		-1.0f, 1.0f, 0.0f,		
		1.0f, 1.0f, 0.0f,	
	};

	glGenVertexArrays(1, &m_rectVAO);

	glGenBuffers(1, &m_rectVBO);
	glBindVertexArray(m_rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

Drawer::~Drawer()
{
}

void Drawer::clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_state = State();
}

void Drawer::translate(int32_t x, int32_t y)
{
	m_state.m_translate_x += x;
	m_state.m_translate_y += y;
}

void Drawer::setScissor(int32_t x, int32_t y, int32_t width, int32_t height)
{
	m_state.m_scissorX = x + m_state.m_translate_x;
	m_state.m_scissorY = y + m_state.m_translate_y;
	m_state.m_scissorWidth = width;
	m_state.m_scissorHeight = height;
}

void Drawer::drawRectange(int32_t x, int32_t y, int32_t width, int32_t height, Color color)
{
	m_rectShader.use();

	float glWidth = static_cast<float>(width) / m_width;
	float glHeight = static_cast<float>(height) / m_height;
	glm::mat4 projection(1.0f);
	projection = glm::translate(projection, glm::vec3(
		-1.0f + static_cast<float>(x + m_state.m_translate_x) / m_width * 2 + glWidth,
		-1.0f + static_cast<float>(y + m_state.m_translate_y) / m_height * 2 + glHeight,
		0.0f
	));
	projection = glm::scale(projection, glm::vec3(glWidth, glHeight, 1.0f));
	glm::vec4 clr;
	clr.a = color.A;
	clr.r = color.R;
	clr.b = color.B;
	clr.g = color.G;

	m_rectShader.setMatrix4fv("projection", projection);
	m_rectShader.set4fv("color", clr);

	glBindVertexArray(m_rectVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Drawer::drawText(int32_t x, int32_t y, int32_t size, Color color, const std::wstring &text)
{
	m_charShader.use();
	if (m_state.m_scissorWidth > 0 && m_state.m_scissorHeight > 0)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_state.m_scissorX, m_state.m_scissorY, m_state.m_scissorWidth, m_state.m_scissorHeight);
	}

	float m_advance = 0;
	for (wchar_t ch : text)
	{
		Character &c = m_font.get(ch);

		float charScale = static_cast<float>(size) / c.m_size;
		float glWidth = charScale * c.m_width / m_width;
		float glHeight = charScale * c.m_height / m_height;
		glm::mat4 projection(1.0f);
		projection = glm::translate(projection, glm::vec3(
			-1.0f + static_cast<float>(x + m_state.m_translate_x + (m_advance + c.m_bearingX) * charScale) / m_width * 2 + glWidth,
			-1.0f + static_cast<float>(y + m_state.m_translate_y - (c.m_height - c.m_bearingY) * charScale) / m_height * 2 + glHeight,
			0.0f
		));
		projection = glm::scale(projection, glm::vec3(glWidth, glHeight, 1.0f));
		glm::vec4 clr;
		clr.a = color.A;
		clr.r = color.R;
		clr.b = color.B;
		clr.g = color.G;

		m_charShader.setMatrix4fv("projection", projection);
		m_charShader.set4fv("color", clr);

		glBindTexture(GL_TEXTURE_2D, c.m_texture);
		glBindVertexArray(m_rectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		m_advance += static_cast<float>(c.m_advance) / 64;
	}

	glDisable(GL_SCISSOR_TEST);
}

Drawer::State Drawer::state() const
{
	return m_state;
}

void Drawer::setState(const State &state)
{
	m_state = state;
}

void Drawer::setSize(int32_t width, int32_t height)
{
	m_width = width;
	m_height = height;
}

int32_t Drawer::width() const
{
	return m_width;
}

int32_t Drawer::height() const
{
	return m_height;
}

