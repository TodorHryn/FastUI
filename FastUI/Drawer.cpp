#include "Drawer.h"
#include "View.h"
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <GLFW\glfw3.h>
#include <glad\glad.h>
#include <algorithm>

Drawer::Drawer(GLFWwindow *window, int32_t width, int32_t height)
	: m_window(window)
	, m_width(width)
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

void Drawer::onCharInput(wchar_t ch)
{
	if (m_focusedView)
		m_focusedView->onCharInput(ch);
}

void Drawer::onKeyboardEvent(const KeyboardEvent &ev)
{
	if (m_focusedView)
		m_focusedView->onKeyboardEvent(ev);
}

void Drawer::onScroll(int32_t x, int32_t y, double xoffset, double yoffset)
{
	m_root->onScroll(x, y, xoffset, yoffset);
}

void Drawer::clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_state = State();
}

void Drawer::render()
{
	clear();
	m_root->draw(m_width, m_height);
	glfwSwapBuffers(m_window);
}

void Drawer::translate(int32_t x, int32_t y)
{
	m_state.m_translate_x += x;
	m_state.m_translate_y += y;
}

void Drawer::translateTo(int32_t x, int32_t y)
{
	m_state.m_translate_x = x;
	m_state.m_translate_y = y;
}

void Drawer::focus(std::shared_ptr<View> view)
{
	m_focusedView = view;
}

void Drawer::setRoot(std::shared_ptr<View> view)
{
	m_root = view;
	m_root->setDrawer(shared_from_this());
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

	glm::mat4 projection(1.0f);
	projection = glm::translate(projection, glm::vec3(
		glx(x + m_state.m_translate_x, width),
		gly(y + m_state.m_translate_y, height),
		0.0f
	));
	projection = glm::scale(projection, glm::vec3(glwidth(width), glheight(height), 1.0f));
	glm::vec4 clr;
	clr.a = color.A / 255.0f;
	clr.r = color.R / 255.0f;
	clr.b = color.B / 255.0f;
	clr.g = color.G / 255.0f;

	m_rectShader.setMatrix4fv("projection", projection);
	m_rectShader.set4fv("color", clr);

	glBindVertexArray(m_rectVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Drawer::drawText(int32_t x, int32_t y, int32_t size, Color color, const std::wstring &text, int32_t cursorPos)
{
	enableScissor();
	float advanceX = 0;
	for (size_t i = 0; i < text.size(); ++i)
	{
		if (cursorPos == i)
			drawRectange(x + advanceX, y, 2, size, Drawer::Color(0x00, 0x00, 0x00));

		m_charShader.use();
		Character &c = m_font.get(text[i]);
		float charScale = static_cast<float>(size) / c.m_size;
		drawChar(c, x + advanceX, y, size, color);
		advanceX += static_cast<float>(c.m_advance) / 64 * charScale;
	}
	if (cursorPos >= 0 && cursorPos >= text.size())
		drawRectange(x + advanceX, y, 2, size, Drawer::Color(0x00, 0x00, 0x00));
	disableScissor();
}

void Drawer::drawText(int32_t x, int32_t y, int32_t width, int32_t height, int32_t size, Color color, const std::wstring &text, int32_t cursorPos)
{
	enableScissor();
	float wordWidth = 0;
	int32_t lineHeight = 0;
	float advanceX = 0, advanceY = 0;
	for (size_t i = 0; i < text.size(); ++i)
	{
		if (cursorPos == i)
			drawRectange(x + advanceX, y + advanceY, 2, size, Drawer::Color(0x00, 0x00, 0x00));

		m_charShader.use();
		if (wordWidth == 0 && text[i] != L' ' && text[i] != L'\n')
		{
			size_t j = i;
			for (; j < text.size() && text[j] != L' ' && text[j] != L'\n'; ++j)
			{
				Character &c = m_font.get(text[j]);
				float charScale = static_cast<float>(size) / c.m_size;
				wordWidth += c.m_advance / 64.0f * charScale;
			}
			if (j > i)
			{
				Character &c = m_font.get(text[j - 1]);
				float charScale = static_cast<float>(size) / c.m_size;
				wordWidth += c.m_width * charScale;
			}

			if (advanceX + wordWidth > width)
			{
				advanceX = 0;
				advanceY += lineHeight * 1.2;
				lineHeight = 0;
			}
		}
		if (text[i] == L' ')
			wordWidth = 0;
		if (text[i] == L'\n')
		{
			advanceX = 0;
			advanceY += lineHeight > 0 ? lineHeight * 1.2 : size * 1.2;
			lineHeight = 0;
			wordWidth = 0;
		}

		if (text[i] != L'\n')
		{
			Character &c = m_font.get(text[i]);
			float charScale = static_cast<float>(size) / c.m_size;
			if (advanceX + (c.m_width + c.m_advance / 64) * charScale > width)
			{
				advanceX = 0;
				advanceY += lineHeight > 0 ? lineHeight * 1.2 : size * 1.2;
				lineHeight = 0;
			}
			lineHeight = std::max(lineHeight, static_cast<int32_t>(c.m_height * charScale));

			drawChar(c, x + advanceX, y + advanceY, size, color);
			advanceX += c.m_advance / 64.0f * charScale;
		}
	}
	if (cursorPos >= 0 && cursorPos >= text.size())
		drawRectange(x + advanceX, y + advanceY, 2, size, Drawer::Color(0x00, 0x00, 0x00));
	disableScissor();
}

std::pair<int32_t, int32_t> Drawer::measureText(int32_t size, const std::wstring &text)
{
	float width = 0, height = 0;
	for (wchar_t ch : text)
	{
		Character &c = m_font.get(ch);
		float charScale = static_cast<float>(size) / c.m_size;
		width += (c.m_width + c.m_advance) / 64 * charScale;
		height = std::max(height, c.m_height * charScale);
	}

	return std::make_pair(width, height);
}

std::pair<int32_t, int32_t> Drawer::measureText(int32_t width, int32_t size, const std::wstring & text)
{
	float retWidth = 0;
	float wordWidth = 0;
	int32_t lineHeight = 0;
	float advanceX = 0, advanceY = 0;
	for (size_t i = 0; i < text.size(); ++i)
	{
		if (wordWidth == 0 && text[i] != L' ' && text[i] != L'\n')
		{
			size_t j = i;
			for (; j < text.size() && text[j] != L' ' && text[j] != L'\n'; ++j)
			{
				Character &c = m_font.get(text[j]);
				float charScale = static_cast<float>(size) / c.m_size;
				wordWidth += c.m_advance / 64.0f * charScale;
			}
			if (j > i)
			{
				Character &c = m_font.get(text[j - 1]);
				float charScale = static_cast<float>(size) / c.m_size;
				wordWidth += c.m_width * charScale;
			}

			if (advanceX + wordWidth > width)
			{
				advanceX = 0;
				advanceY += lineHeight * 1.2;
				lineHeight = 0;
			}
		}
		if (text[i] == L' ')
			wordWidth = 0;
		if (text[i] == L'\n')
		{
			advanceX = 0;
			advanceY += lineHeight > 0 ? lineHeight * 1.2 : size * 1.2;
			lineHeight = 0;
			wordWidth = 0;
		}

		if (text[i] != L'\n')
		{
			Character &c = m_font.get(text[i]);
			float charScale = static_cast<float>(size) / c.m_size;
			if (advanceX + (c.m_width + c.m_advance / 64) * charScale > width)
			{
				advanceX = 0;
				advanceY += lineHeight > 0 ? lineHeight * 1.2 : size * 1.2;
				lineHeight = 0;
			}
			lineHeight = std::max(lineHeight, static_cast<int32_t>(c.m_height * charScale));

			advanceX += c.m_advance / 64.0f * charScale;
			retWidth = std::max(retWidth, advanceX);
		}
	}
	return std::make_pair(retWidth, advanceY + size);
}

const Drawer::State& Drawer::state() const
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

bool Drawer::isFocused(std::shared_ptr<View> view)
{
	return m_focusedView == view;
}

int32_t Drawer::getTimeMs() const
{
	return glfwGetTime() * 1000;
}

float Drawer::glx(int32_t x, int32_t width)
{
	return -1.0f + static_cast<float>(x + width / 2) / m_width * 2;
}

float Drawer::gly(int32_t y, int32_t height)
{
	return 1.0f - static_cast<float>(y + height / 2) / m_height * 2;
}

float Drawer::glwidth(int32_t width)
{
	return static_cast<float>(width) / m_width;
}

float Drawer::glheight(int32_t height)
{
	return static_cast<float>(height) / m_height;
}

void Drawer::enableScissor()
{
	if (m_state.m_scissorWidth > 0 && m_state.m_scissorHeight > 0)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_state.m_scissorX, m_height - m_state.m_scissorY - m_state.m_scissorHeight, m_state.m_scissorWidth, m_state.m_scissorHeight);
	}
}

void Drawer::disableScissor()
{
	glDisable(GL_SCISSOR_TEST);
}

void Drawer::drawChar(const Character &c, int32_t x, int32_t y, int32_t size, Color color)
{
	float charScale = static_cast<float>(size) / c.m_size;
	glm::mat4 projection(1.0f);
	projection = glm::translate(projection, glm::vec3(
		glx(x + m_state.m_translate_x + c.m_bearingX * charScale, charScale * c.m_width),
		gly(y + m_state.m_translate_y - c.m_bearingY * charScale + size, charScale * c.m_height),
		0.0f
	));
	projection = glm::scale(projection, glm::vec3(glwidth(charScale * c.m_width), glheight(charScale * c.m_height), 1.0f));
	glm::vec4 clr;
	clr.a = color.A / 255.0f;
	clr.r = color.R / 255.0f;
	clr.b = color.B / 255.0f;
	clr.g = color.G / 255.0f;

	m_charShader.setMatrix4fv("projection", projection);
	m_charShader.set4fv("color", clr);

	glBindTexture(GL_TEXTURE_2D, c.m_texture);
	glBindVertexArray(m_rectVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

