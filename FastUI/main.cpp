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

std::shared_ptr<LinearLayout> createLayout(bool horz, int recDepth,	int dir = 0)
{
	std::shared_ptr<LinearLayout> layout = std::make_shared<LinearLayout>(View::SizePolitics::MAX, View::SizePolitics::MAX);
	if (horz)
		layout->setOrientation(LinearLayout::Orientation::HORIZONTAL);
	layout->setSpacing(2);

	std::shared_ptr<Button> btn = std::make_shared<Button>(View::SizePolitics::MAX, View::SizePolitics::MAX);
	btn->setTextSize(24).setText(L"∆ми сюды!");

	if (dir % 3 == 0)
		btn->setBackgroundColor(Drawer::Color(0xFF));
	else if (dir % 3 == 1)
		btn->setBackgroundColor(Drawer::Color(0x00, 0xFF));
	else
		btn->setBackgroundColor(Drawer::Color(0x00, 0x00, 0xFF));
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
	static auto layout = createLayout(true, 10);

	drawer->clear();
	layout->draw(*drawer, drawer->width(), drawer->height());
	glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	drawer->setSize(width, height);
	draw();
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawer = std::make_shared<Drawer>(1280, 720);
	while (!glfwWindowShouldClose(window)) {
		draw();
		glfwPollEvents();		
	}
	
	glfwTerminate();
	return 0;
}