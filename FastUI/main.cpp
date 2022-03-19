#include <iostream>
#include <glad\glad.h>
#include <glad.c>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "ShaderProgram.h"
#include "Button.h"
#include "LinearLayout.h"
#include "Font.h"

GLFWwindow *window;
std::shared_ptr<Drawer> drawer;
std::shared_ptr<LinearLayout> mainLayout;

std::shared_ptr<LinearLayout> createLayout(bool horz, int recDepth,	int dir = 0)
{
	std::shared_ptr<LinearLayout> layout = std::make_shared<LinearLayout>(View::SizePolitics::MAX, View::SizePolitics::MAX);
	if (horz)
		layout->setOrientation(LinearLayout::Orientation::HORIZONTAL);
	layout->setSpacing(2);

	std::shared_ptr<Button> btn = std::make_shared<Button>(View::SizePolitics::MAX, View::SizePolitics::MAX);
	btn->setTextSize(24).setText(L"∆ми сюды!");
	btn->setOnMousePress([btn] (const MouseEvent &ev) {
		if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
		{
			btn->m_backgroundColor = Drawer::Color(0xFF - btn->m_backgroundColor.R, 0xFF - btn->m_backgroundColor.G, 0xFF - btn->m_backgroundColor.B, 0xFF);
			return true;
		}
	});

	if (dir % 3 == 0)
		btn->m_backgroundColor = Drawer::Color(0xFF);
	else if (dir % 3 == 1)
		btn->m_backgroundColor = Drawer::Color(0x00, 0xFF);
	else
		btn->m_backgroundColor = Drawer::Color(0x00, 0x00, 0xFF);
	if (dir == 0 || dir == 3)
		layout->addChild(btn);

	if (recDepth > 0)
	{
		std::shared_ptr<LinearLayout> lay = createLayout(!horz, recDepth - 1, (dir + 1) % 4);
		layout->addChild(lay);
	}

	if (!(dir == 0 || dir == 3))
		layout->addChild(btn);

	return layout;
}

void draw()
{
	drawer->clear();
	mainLayout->draw(*drawer, drawer->width(), drawer->height());
	glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	drawer->setSize(width, height);
	draw();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	y = drawer->height() - y;

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

	mainLayout->onMousePress(MouseEvent(btn, act, x, y));
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(1280, 720, "Hello, world!", NULL, NULL);
	if (window == nullptr) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 1280, 720);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawer = std::make_shared<Drawer>(1280, 720);
	mainLayout = createLayout(true, 10);
	while (!glfwWindowShouldClose(window)) {
		draw();
		glfwPollEvents();		
	}
	
	glfwTerminate();
	return 0;
}