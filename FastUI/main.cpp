#include <iostream>
#include <glad\glad.h>
#include <glad\glad.c>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "ShaderProgram.h"
#include "Button.h"
#include "LinearLayout.h"

const int width = 1280, height = 720;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process(Drawer &drawer)
{
	drawer.clear();

	static int w = width, h = height;
	LinearLayout layout(View::SizePolitics::MAX, View::SizePolitics::MAX);
	layout.setOrientation(LinearLayout::Orientation::HORIZONTAL);
	layout.setSpacing(10);
	
	std::shared_ptr<Button> btn1 = std::make_shared<Button>(View::SizePolitics::MAX, View::SizePolitics::MAX);
	btn1->setBackgroundColor(Drawer::Color(0xFF));
	layout.addChild(btn1);

	std::shared_ptr<Button> btn2 = std::make_shared<Button>(View::SizePolitics::MAX, View::SizePolitics::MAX);
	btn2->setBackgroundColor(Drawer::Color(0x00, 0xFF));
	layout.addChild(btn2);

	std::shared_ptr<LinearLayout> lay1 = std::make_shared<LinearLayout>(View::SizePolitics::MAX, View::SizePolitics::MAX);
	lay1->setSpacing(10);
	layout.addChild(lay1);
	std::shared_ptr<Button> btn3 = std::make_shared<Button>(View::SizePolitics::MAX, View::SizePolitics::MAX);
	btn3->setBackgroundColor(Drawer::Color(0x00, 0x00, 0xFF));
	lay1->addChild(btn3);
	std::shared_ptr<Button> btn4 = std::make_shared<Button>(View::SizePolitics::MAX, View::SizePolitics::MAX);
	btn4->setBackgroundColor(Drawer::Color(0xFF, 0x00, 0x00, 0x70));
	lay1->addChild(btn4);

	layout.draw(drawer, w, h);
	w--;
	h--;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(width, height, "Hello, world!", NULL, NULL);
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

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderProgram shader("shader");
	float rect[] = {
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 1.0f,

		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 1.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	float prevTime = glfwGetTime();

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	Drawer drawer(width, height);

	while (!glfwWindowShouldClose(window)) {
		float dt = glfwGetTime() - prevTime;
		prevTime = glfwGetTime();

		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		process(drawer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, drawer.get());
		glGenerateMipmap(GL_TEXTURE_2D);

		glm::mat4 projection(1.0f);
		projection = glm::scale(projection, glm::vec3(1.0f, 1.0f, 1.0f));

		shader.use();
		shader.setMatrix4fv("projection", projection);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();		
	}
	
	glfwTerminate();
	return 0;
}