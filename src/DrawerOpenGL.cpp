#include "DrawerOpenGL.h"
#include "ImageOpenGL.h"
#include "View.h"
#include "Util.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <algorithm>

namespace fastui
{
	Drawer* drawer = nullptr;
	size_t ROUNDED_CORNER_POINTS = 4;

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

			MouseEvent::Button btn(MouseEvent::Button::NONE);
			MouseEvent::Action act(MouseEvent::Action::NONE);

			if (button == GLFW_MOUSE_BUTTON_LEFT)
				btn = MouseEvent::Button::LEFT;
			else if (button == GLFW_MOUSE_BUTTON_RIGHT)
				btn = MouseEvent::Button::RIGHT;

			if (action == GLFW_PRESS)
			{
				act = MouseEvent::Action::PRESS;
				drawer->focus(std::shared_ptr<View>());
			}
			else if (action == GLFW_RELEASE)
				act = MouseEvent::Action::RELEASE;

			drawer->onMouseEvent(MouseEvent(btn, act, static_cast<int32_t>(x), static_cast<int32_t>(y)));
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
			KeyboardEvent::Button btn(KeyboardEvent::Button::NONE);
			KeyboardEvent::Action act(KeyboardEvent::Action::NONE);

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
			drawer->onScroll(static_cast<int32_t>(x), static_cast<int32_t>(y), xoffset, yoffset);
		}
	}

	void cursor_position_callback(GLFWwindow* window, double x, double y)
	{
		if (drawer)
		{
			drawer->onMouseMove(static_cast<int32_t>(x), static_cast<int32_t>(y));
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

		m_window = glfwCreateWindow(width, height, "FastUI Project", NULL, NULL);
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
		glfwSetCursorPosCallback(m_window, cursor_position_callback);
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

		std::vector<float> roundedCorner;
		roundedCorner.push_back(-1.0f);
		roundedCorner.push_back(-1.0f);
		roundedCorner.push_back(0.0f);
		for (size_t i = 0; i < ROUNDED_CORNER_POINTS; ++i)
		{
			float angle = i * 3.1415 / 2 / (ROUNDED_CORNER_POINTS - 1);
			roundedCorner.push_back(sin(angle) * 2 - 1);
			roundedCorner.push_back(cos(angle) * 2 - 1);
			roundedCorner.push_back(0.0f);
		}
		glGenVertexArrays(1, &m_cornerVAO);
		glGenBuffers(1, &m_cornerVBO);
		glBindVertexArray(m_cornerVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_cornerVBO);
		glBufferData(GL_ARRAY_BUFFER, roundedCorner.size() * sizeof(float), roundedCorner.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		m_charShader.load("char");
		m_rectShader.load("shader");
		m_imageShader.load("image");

		m_fonts.resize(3);
		m_fonts[0].load("OpenSans-Regular.ttf");
		m_fonts[1].load("NotoEmoji-Regular.ttf");
		m_fonts[2].load("NotoSansSymbols2-Regular.ttf");
	}

	DrawerOpenGL::~DrawerOpenGL()
	{
		glfwTerminate();
	}

	void DrawerOpenGL::setTitle(const UnicodeString& title) 
	{
		glfwSetWindowTitle(m_window, title.toUtf8().c_str());
	}

	void DrawerOpenGL::execute()
	{
		while (!glfwWindowShouldClose(m_window)) {
			double x, y;
			glfwGetCursorPos(m_window, &x, &y);
			std::shared_ptr<View> newViewAtMousePos = m_root->getViewOverlayAtMousePos(x, y);
			if (newViewAtMousePos)
			{
				if (newViewAtMousePos != m_viewAtMousePos.first || !m_viewAtMousePos.second)
				{
					if (m_viewAtMousePos.first)
					{
						if (m_viewAtMousePos.second) m_viewAtMousePos.first->onMouseEventOverlay(MouseEvent(MouseEvent::Action::LEAVE));
						else m_viewAtMousePos.first->onMouseEvent(MouseEvent(MouseEvent::Action::LEAVE));
					}
					m_viewAtMousePos = std::make_pair(newViewAtMousePos, true);
					m_viewAtMousePos.first->onMouseEventOverlay(MouseEvent(MouseEvent::Action::ENTER));
				}
			}
			else
			{
				newViewAtMousePos = m_root->getViewAtMousePos(x, y);
				if (newViewAtMousePos != m_viewAtMousePos.first || m_viewAtMousePos.second)
				{
					if (m_viewAtMousePos.first)
					{
						if (m_viewAtMousePos.second) m_viewAtMousePos.first->onMouseEventOverlay(MouseEvent(MouseEvent::Action::LEAVE));
						else m_viewAtMousePos.first->onMouseEvent(MouseEvent(MouseEvent::Action::LEAVE));
					}
					m_viewAtMousePos = std::make_pair(newViewAtMousePos, false);
					m_viewAtMousePos.first->onMouseEvent(MouseEvent(MouseEvent::Action::ENTER));
				}
			}

			double prevTime = glfwGetTime();
			drawer->render();
			glfwPollEvents();
			int32_t time = static_cast<int32_t>((1 / 60.0f - glfwGetTime() + prevTime) * 1000 * 1000);
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

	void DrawerOpenGL::drawRoundedRectangle(int32_t x, int32_t y, int32_t width, int32_t height, Color color)
	{
		int32_t radius = 4;

		drawRoundedCorner(x, y, radius, radius, color, 3.1415 / 2);
		drawRoundedCorner(width - radius, y, radius, radius, color, 0);
		drawRoundedCorner(x, height - radius, radius, radius, color, 3.1415);
		drawRoundedCorner(width - radius, height - radius, radius, radius, color, 3.1415 * 3 / 2);
		drawRectangle(x + radius, y, width - radius * 2, radius, color);
		drawRectangle(x, y + radius, width, height - radius * 2, color);
		drawRectangle(x + radius, height - radius, width - radius * 2, radius, color);
	}

	void DrawerOpenGL::drawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, Color color)
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

	void DrawerOpenGL::drawShadowBorder(int32_t x, int32_t y, int32_t width, int32_t height, int32_t thickness, Color color)
	{
		drawRectangle(width - thickness, 0, thickness, height, color);		//Right
		drawRectangle(0, 0, thickness, height, color);						//Left
		drawRectangle(0, 0, width, thickness, color);						//Top
		drawRectangle(0, height, width, thickness, color);					//Bottom
	}

	void DrawerOpenGL::drawText(int32_t x, int32_t baselineY, int32_t size, Color color, const UnicodeString& text, int32_t cursorPos)
	{
		enableScissor();
		float advanceX = 0;
		for (size_t i = 0; i < text.size(); i++)
		{
			if (cursorPos == i)
				drawRectangle(x + static_cast<int32_t>(advanceX), baselineY, 2, -size, Drawer::Color(0x00, 0x00, 0x00));

			m_charShader.use();
			CharacterOpenGL& c = getChar(text[i]);
			float charScale = static_cast<float>(size) / c.m_size;
			drawChar(c, x + static_cast<int32_t>(advanceX), baselineY, size, color);
			advanceX += static_cast<float>(c.m_advance) / 64 * charScale;
		}
		if (cursorPos >= 0 && cursorPos >= text.size())
			drawRectangle(x + static_cast<int32_t>(advanceX), baselineY, 2, -size, Drawer::Color(0x00, 0x00, 0x00));
		disableScissor();
	}

	void DrawerOpenGL::drawText(int32_t x, int32_t baselineY, int32_t width, int32_t height, int32_t size, Color color, const UnicodeString& text, int32_t cursorPos)
	{
		enableScissor();
		float wordWidth = 0;
		float advanceX = 0, advanceY = 0;
		for (size_t i = 0; i < text.size(); i++)
		{
			if (cursorPos == i)
				drawRectangle(x + static_cast<int32_t>(advanceX), baselineY + static_cast<int32_t>(advanceY), 2, -size, Drawer::Color(0x00, 0x00, 0x00));

			m_charShader.use();
			if (wordWidth == 0 && text[i] != ' ' && text[i] != '\n')
			{
				size_t j = i;
				for (; j < text.size() && text[j] != ' ' && text[j] != '\n'; j++)
				{
					CharacterOpenGL& c = getChar(text[j]);
					float charScale = static_cast<float>(size) / c.m_size;
					wordWidth += c.m_advance / 64.0f * charScale;
				}
				if (j > i)
				{
					CharacterOpenGL& c = getChar(text[j - 1]);
					float charScale = static_cast<float>(size) / c.m_size;
					wordWidth += c.m_width * charScale;
				}

				if (advanceX + wordWidth > width && wordWidth < width)
				{
					advanceX = 0;
					advanceY += size;
				}
			}
			if (text[i] == ' ')
				wordWidth = 0;
			if (text[i] == '\n')
			{
				advanceX = 0;
				advanceY += size;
				wordWidth = 0;
			}

			if (text[i] != '\n')
			{
				CharacterOpenGL& c = getChar(text[i]);
				float charScale = static_cast<float>(size) / c.m_size;
				if (advanceX + (c.m_width + c.m_advance / 64) * charScale > width)
				{
					advanceX = 0;
					advanceY += size;
				}

				drawChar(c, x + static_cast<int32_t>(advanceX), baselineY + static_cast<int32_t>(advanceY), size, color);
				advanceX += c.m_advance / 64.0f * charScale;
			}
		}
		if (cursorPos >= 0 && cursorPos >= text.size())
			drawRectangle(x + static_cast<int32_t>(advanceX), baselineY + static_cast<int32_t>(advanceY), 2, -size, Drawer::Color(0x00, 0x00, 0x00));
		disableScissor();
	}

	void DrawerOpenGL::drawImage(int32_t x, int32_t y, int32_t width, int32_t height, const Image& i)
	{
		enableScissor();
		m_imageShader.use();

		const ImageOpenGL& img = static_cast<const ImageOpenGL&>(i);
		glm::mat4 projection(1.0f);
		projection = glm::translate(projection, glm::vec3(
			glx(x + m_state.m_translate_x, width),
			gly(y + m_state.m_translate_y, height),
			0.0f
		));
		projection = glm::scale(projection, glm::vec3(glwidth(width), glheight(height), 1.0f));

		m_imageShader.setMatrix4fv("projection", projection);

		glBindVertexArray(m_rectVAO);
		glBindTexture(GL_TEXTURE_2D, img.m_texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		disableScissor();
	}

	DrawerOpenGL::TextSize DrawerOpenGL::measureText(int32_t size, const UnicodeString& text)
	{
		TextSize ret;
		ret.minY = 1e9;

		for (size_t i = 0; i < text.size(); ++i)
		{
			CharacterOpenGL& c = getChar(text[i]);
			float charScale = static_cast<float>(size) / c.m_size;
			ret.width += (c.m_width + c.m_advance) / 64 * charScale;

			ret.maxY = std::max(ret.maxY, static_cast<int32_t>(ceil(c.m_bearingY * charScale)));
			ret.minY = std::min(ret.minY, static_cast<int32_t>(floor((c.m_bearingY - c.m_height) * charScale)));
		}

		return ret;
	}

	DrawerOpenGL::TextSize DrawerOpenGL::measureText(int32_t width, int32_t size, const UnicodeString& text)  //TODO (fix): calc height by maxY - minY
	{
		TextSize ret;
		float wordWidth = 0;
		float advanceX = 0, advanceY = 0;
		for (size_t i = 0; i < text.size(); ++i)
		{
			if (wordWidth == 0 && text[i] != ' ' && text[i] != '\n')
			{
				size_t j = i;
				for (; j < text.size() && text[j] != ' ' && text[j] != '\n'; ++j)
				{
					CharacterOpenGL& c = getChar(text[j]);
					float charScale = static_cast<float>(size) / c.m_size;
					wordWidth += c.m_advance / 64.0f * charScale;
				}
				if (j > i)
				{
					CharacterOpenGL& c = getChar(text[j - 1]);
					float charScale = static_cast<float>(size) / c.m_size;
					wordWidth += c.m_width * charScale;
				}

				if (advanceX + wordWidth > width && wordWidth < width)
				{
					advanceX = 0;
					advanceY += size;
				}
			}
			if (text[i] == ' ')
				wordWidth = 0;
			if (text[i] == '\n')
			{
				advanceX = 0;
				advanceY += size;
				wordWidth = 0;
			}

			if (text[i] != '\n')
			{
				CharacterOpenGL& c = getChar(text[i]);
				float charScale = static_cast<float>(size) / c.m_size;
				if (advanceX + (c.m_width + c.m_advance / 64) * charScale > width)
				{
					advanceX = 0;
					advanceY += size;
				}

				advanceX += c.m_advance / 64.0f * charScale;
				ret.width = std::max(ret.width, static_cast<int32_t>(advanceX));
			}
		}
		ret.maxY = static_cast<int32_t>(advanceY + size);
		return ret;
	}

	CharacterOpenGL& DrawerOpenGL::getChar(UnicodeString::char_type ch)
	{
		for (size_t i = 0; i < m_fonts.size(); ++i)
			if (m_fonts[i].contains(ch))
				return m_fonts[i].get(ch);
		return m_fonts[0].get(0xFFFD);
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

	void DrawerOpenGL::drawRoundedCorner(int32_t x, int32_t y, int32_t width, int32_t height, Color color, float angle)
	{
		m_rectShader.use();

		glm::mat4 projection(1.0f);
		projection = glm::translate(projection, glm::vec3(
			glx(x + m_state.m_translate_x, width),
			gly(y + m_state.m_translate_y, height),
			0.0f
		));
		projection = glm::scale(projection, glm::vec3(glwidth(width), glheight(height), 1.0f));
		projection = glm::rotate(projection, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec4 clr;
		clr.a = color.A / 255.0f;
		clr.r = color.R / 255.0f;
		clr.b = color.B / 255.0f;
		clr.g = color.G / 255.0f;

		m_rectShader.setMatrix4fv("projection", projection);
		m_rectShader.set4fv("color", clr);

		glBindVertexArray(m_cornerVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, ROUNDED_CORNER_POINTS + 1);
	}

	void DrawerOpenGL::drawChar(const CharacterOpenGL& c, int32_t x, int32_t y, int32_t size, Color color)
	{
		float charScale = static_cast<float>(size) / c.m_size;
		glm::mat4 projection(1.0f);
		projection = glm::translate(projection, glm::vec3(
			glx(static_cast<int32_t>(x + m_state.m_translate_x + c.m_bearingX * charScale), static_cast<int32_t>(charScale * c.m_width)),
			gly(static_cast<int32_t>(y + m_state.m_translate_y - c.m_bearingY * charScale), static_cast<int32_t>(charScale * c.m_height)),
			0.0f
		));
		projection = glm::scale(projection, glm::vec3(glwidth(static_cast<int32_t>(charScale * c.m_width)), glheight(static_cast<int32_t>(charScale * c.m_height)), 1.0f));
		glm::vec4 clr;
		clr.a = color.A / 255.0f;
		clr.r = color.R / 255.0f;
		clr.b = color.B / 255.0f;
		clr.g = color.G / 255.0f;

		m_charShader.setMatrix4fv("projection", projection);
		m_charShader.set4fv("color", clr);

		glBindVertexArray(m_rectVAO);
		glBindTexture(GL_TEXTURE_2D, c.m_texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	float DrawerOpenGL::glx(int32_t x, int32_t width)
	{
		return -1.0f + (x + static_cast<float>(width) / 2) / m_width * 2;
	}

	float DrawerOpenGL::gly(int32_t y, int32_t height)
	{
		return 1.0f - (y + static_cast<float>(height) / 2) / m_height * 2;
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
		return static_cast<int32_t>(glfwGetTime() * 1000);
	}
};
