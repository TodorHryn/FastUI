#include "DrawerOpenGL.h"
#include "View.h"
#include "Util.h"
#include <glad\glad.h>
#include <glad.c>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <algorithm>

Drawer* drawer = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	if (drawer)
	{
		glViewport(0, 0, width, height);
		drawer->setSize(width, height);
		drawer->render();
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (drawer)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		MouseEvent::Button btn;
		MouseEvent::Action act;

		if (button == GLFW_MOUSE_BUTTON_LEFT)
			btn = MouseEvent::Button::LEFT;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			btn = MouseEvent::Button::RIGHT;

		if (action == GLFW_PRESS)
			act = MouseEvent::Action::PRESS;
		else if (action == GLFW_RELEASE)
			act = MouseEvent::Action::RELEASE;

		drawer->onMouseEvent(MouseEvent(btn, act, x, y));
	}
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	if (drawer)
		drawer->onCharInput(codepoint);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (drawer)
	{
		KeyboardEvent::Button btn = KeyboardEvent::Button::NONE;
		KeyboardEvent::Action act;

		if (action == GLFW_PRESS)
			act = KeyboardEvent::Action::PRESS;
		else if (action == GLFW_RELEASE)
			act = KeyboardEvent::Action::RELEASE;
		else if (action == GLFW_REPEAT)
			act = KeyboardEvent::Action::REPEAT;

		if (key == GLFW_KEY_BACKSPACE)
			btn = KeyboardEvent::Button::BACKSPACE;
		else if (key == GLFW_KEY_LEFT)
			btn = KeyboardEvent::Button::LEFT;
		else if (key == GLFW_KEY_RIGHT)
			btn = KeyboardEvent::Button::RIGHT;
		else if (key == GLFW_KEY_ENTER)
			btn = KeyboardEvent::Button::ENTER;

		if (btn != KeyboardEvent::Button::NONE)
			drawer->onKeyboardEvent(KeyboardEvent(btn, act));
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (drawer)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		drawer->onScroll(x, y, xoffset, yoffset);
	}
}

DrawerOpenGL::DrawerOpenGL(int32_t width, int32_t height)
	: Drawer(width, height)
{
	drawer = this;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, "Hello, world!", NULL, NULL);
	if (m_window == nullptr) {
		glfwTerminate();
		throw std::exception("Failed to create window");
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw  std::exception("Failed to initialize GLAD");

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetCharCallback(m_window, character_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	m_rectShader.load("shader");
	m_charShader.load("char");
	m_font.setFontPath(L"OpenSans-Regular.ttf");
}

DrawerOpenGL::~DrawerOpenGL()
{
	glfwTerminate();
}

void DrawerOpenGL::execute()
{
	while (!glfwWindowShouldClose(m_window)) {
		double prevTime = glfwGetTime();
		drawer->render();
		glfwPollEvents();
		int32_t time = (1 / 60.0f - glfwGetTime() + prevTime) * 1000 * 1000;
		if (time >= 0)
			SleepInUs(time);
	}
}

void DrawerOpenGL::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_state = State();

	m_root->draw(m_width, m_height);
	m_root->drawOverlay(m_width, m_height);
	glfwSwapBuffers(m_window);
}

void DrawerOpenGL::drawRectange(int32_t x, int32_t y, int32_t width, int32_t height, Color color)
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

void DrawerOpenGL::drawText(int32_t x, int32_t y, int32_t size, Color color, const std::wstring &text, int32_t cursorPos)
{
	enableScissor();
	float advanceX = 0;
	for (size_t i = 0; i < text.size(); ++i)
	{
		if (cursorPos == i)
			drawRectange(x + advanceX, y, 2, size, Drawer::Color(0x00, 0x00, 0x00));

		m_charShader.use();
		CharacterOpenGL &c = m_font.get(text[i]);
		float charScale = static_cast<float>(size) / c.m_size;
		drawChar(c, x + advanceX, y, size, color);
		advanceX += static_cast<float>(c.m_advance) / 64 * charScale;
	}
	if (cursorPos >= 0 && cursorPos >= text.size())
		drawRectange(x + advanceX, y, 2, size, Drawer::Color(0x00, 0x00, 0x00));
	disableScissor();
}

void DrawerOpenGL::drawText(int32_t x, int32_t y, int32_t width, int32_t height, int32_t size, Color color, const std::wstring &text, int32_t cursorPos)
{
	enableScissor();
	float wordWidth = 0;
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
				CharacterOpenGL &c = m_font.get(text[j]);
				float charScale = static_cast<float>(size) / c.m_size;
				wordWidth += c.m_advance / 64.0f * charScale;
			}
			if (j > i)
			{
				CharacterOpenGL &c = m_font.get(text[j - 1]);
				float charScale = static_cast<float>(size) / c.m_size;
				wordWidth += c.m_width * charScale;
			}

			if (advanceX + wordWidth > width && wordWidth < width)
			{
				advanceX = 0;
				advanceY += size;
			}
		}
		if (text[i] == L' ')
			wordWidth = 0;
		if (text[i] == L'\n')
		{
			advanceX = 0;
			advanceY += size;
			wordWidth = 0;
		}

		if (text[i] != L'\n')
		{
			CharacterOpenGL &c = m_font.get(text[i]);
			float charScale = static_cast<float>(size) / c.m_size;
			if (advanceX + (c.m_width + c.m_advance / 64) * charScale > width)
			{
				advanceX = 0;
				advanceY += size;
			}

			drawChar(c, x + advanceX, y + advanceY, size, color);
			advanceX += c.m_advance / 64.0f * charScale;
		}
	}
	if (cursorPos >= 0 && cursorPos >= text.size())
		drawRectange(x + advanceX, y + advanceY, 2, size, Drawer::Color(0x00, 0x00, 0x00));
	disableScissor();
}

std::pair<int32_t, int32_t> DrawerOpenGL::measureText(int32_t size, const std::wstring &text)
{
	float width = 0, height = 0;
	for (wchar_t ch : text)
	{
		CharacterOpenGL &c = m_font.get(ch);
		float charScale = static_cast<float>(size) / c.m_size;
		width += (c.m_width + c.m_advance) / 64 * charScale;
		height = max(height, c.m_height * charScale);
	}

	return std::make_pair(width, height);
}

std::pair<int32_t, int32_t> DrawerOpenGL::measureText(int32_t width, int32_t size, const std::wstring & text)
{
	float retWidth = 0;
	float wordWidth = 0;
	float advanceX = 0, advanceY = 0;
	for (size_t i = 0; i < text.size(); ++i)
	{
		if (wordWidth == 0 && text[i] != L' ' && text[i] != L'\n')
		{
			size_t j = i;
			for (; j < text.size() && text[j] != L' ' && text[j] != L'\n'; ++j)
			{
				CharacterOpenGL &c = m_font.get(text[j]);
				float charScale = static_cast<float>(size) / c.m_size;
				wordWidth += c.m_advance / 64.0f * charScale;
			}
			if (j > i)
			{
				CharacterOpenGL &c = m_font.get(text[j - 1]);
				float charScale = static_cast<float>(size) / c.m_size;
				wordWidth += c.m_width * charScale;
			}

			if (advanceX + wordWidth > width && wordWidth < width)
			{
				advanceX = 0;
				advanceY += size;
			}
		}
		if (text[i] == L' ')
			wordWidth = 0;
		if (text[i] == L'\n')
		{
			advanceX = 0;
			advanceY += size;
			wordWidth = 0;
		}

		if (text[i] != L'\n')
		{
			CharacterOpenGL &c = m_font.get(text[i]);
			float charScale = static_cast<float>(size) / c.m_size;
			if (advanceX + (c.m_width + c.m_advance / 64) * charScale > width)
			{
				advanceX = 0;
				advanceY += size;
			}

			advanceX += c.m_advance / 64.0f * charScale;
			retWidth = max(retWidth, advanceX);
		}
	}
	return std::make_pair(retWidth, advanceY + size);
}

void DrawerOpenGL::enableScissor()
{
	if (m_state.m_scissorWidth > 0 && m_state.m_scissorHeight > 0)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_state.m_scissorX, m_height - m_state.m_scissorY - m_state.m_scissorHeight, m_state.m_scissorWidth, m_state.m_scissorHeight);
	}
}

void DrawerOpenGL::disableScissor()
{
	glDisable(GL_SCISSOR_TEST);
}

void DrawerOpenGL::drawChar(const CharacterOpenGL &c, int32_t x, int32_t y, int32_t size, Color color)
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

float DrawerOpenGL::glx(int32_t x, int32_t width)
{
	return -1.0f + static_cast<float>(x + width / 2) / m_width * 2;
}

float DrawerOpenGL::gly(int32_t y, int32_t height)
{
	return 1.0f - static_cast<float>(y + height / 2) / m_height * 2;
}

float DrawerOpenGL::glwidth(int32_t width)
{
	return static_cast<float>(width) / m_width;
}

float DrawerOpenGL::glheight(int32_t height)
{
	return static_cast<float>(height) / m_height;
}

int32_t DrawerOpenGL::getTimeMs() const
{
	return glfwGetTime() * 1000;
}
