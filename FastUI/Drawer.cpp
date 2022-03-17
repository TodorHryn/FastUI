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

void Drawer::drawRectange(int32_t x, int32_t y, int32_t width, int32_t height, Color color)
{
	m_rectShader.use();

	glm::mat4 projection(1.0f);
	projection = glm::translate(projection, glm::vec3(
		-1.0f + static_cast<float>(x + m_state.m_translate_x) / m_width * 2 + static_cast<float>(width) / m_width,
		-1.0f + static_cast<float>(y + m_state.m_translate_y) / m_height * 2 + static_cast<float>(height) / m_height,
		0.0f
	));
	projection = glm::scale(projection, glm::vec3(static_cast<float>(width) / m_width, static_cast<float>(height) / m_height, 1.0f));
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
