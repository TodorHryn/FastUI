﻿#include <iostream>
#include <glad\glad.h>
#include <glad.c>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "ShaderProgram.h"
#include "Button.h"
#include "LinearLayout.h"
#include "TextField.h"
#include "TextArea.h"
#include "Font.h"
#include "Util.h"
#include "Spacer.h"

GLFWwindow *window;
std::shared_ptr<Drawer> drawer;
std::shared_ptr<LinearLayout> mainLayout;

std::shared_ptr<LinearLayout> createLayout(bool horz, int recDepth,	int dir = 0)
{
	std::shared_ptr<LinearLayout> layout = std::make_shared<LinearLayout>();
	if (horz)
		layout->setOrientation(LinearLayout::Orientation::HORIZONTAL);
	layout->setSpacing(2);

	std::shared_ptr<Button> btn = std::make_shared<Button>();
	btn->m_textSize = 24;
	btn->m_text = L"Жми сюды!";
	btn->setOnMouseEvent([btn] (const MouseEvent &ev) {
		if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
		{
			btn->m_backgroundColor = Drawer::Color(0xFF - btn->m_backgroundColor.R, 0xFF - btn->m_backgroundColor.G, 0xFF - btn->m_backgroundColor.B, 0xFF);
		}
		return true;
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

std::vector<std::vector<std::wstring>> calcButtons = {
	{ L"7", L"8", L"9", L"+", L"<="},
	{ L"4", L"5", L"6", L"-", L"C"},
	{ L"1", L"2", L"3", L"*", L" "},
	{ L".", L"0", L"=", L"/", L" "}
};
std::unordered_map<wchar_t, int8_t> calcPriorities = {
	{'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}
};

float prevNumber = 0;
std::wstring prevOp = L"";

void processKey(wchar_t btn, std::shared_ptr<TextField> number)
{
	if (btn == L'=')
	{
		std::wstring num;
		std::vector<std::wstring> nums;
		std::vector<wchar_t> ops;

		//RPN
		for (wchar_t ch : number->m_text)
		{
			if ((ch >= L'0' && ch <= L'9') || ch == L'.')
				num += ch;
			else if (ch == L'+' || ch == L'-' || ch == L'*' || ch == L'/')
			{
				nums.push_back(num);
				num = L"";
				if (ops.empty())
					ops.push_back(ch);
				else
				{
					while (ops.size() && calcPriorities[ch] <= calcPriorities[ops.back()])
					{
						std::wstring str;
						str += ops.back();
						nums.push_back(str);
						ops.pop_back();
					}
					ops.push_back(ch);
				}
			}
		}
		if (num.size())
			nums.push_back(num);
		while (ops.size())
		{
			std::wstring str;
			str += ops.back();
			nums.push_back(str);
			ops.pop_back();
		}

		//Calc	
		std::vector<float> floats;
		for (const std::wstring &str : nums)
		{
			if (str == L"+")
			{
				floats[floats.size() - 2] += floats.back();
				floats.pop_back();
			}
			else if (str == L"-")
			{
				floats[floats.size() - 2] -= floats.back();
				floats.pop_back();
			}
			else if (str == L"*")
			{
				floats[floats.size() - 2] *= floats.back();
				floats.pop_back();
			}
			else if (str == L"/")
			{
				floats[floats.size() - 2] /= floats.back();
				floats.pop_back();
			}
			else
			{
				try {
					floats.push_back(std::stof(str));
				}
				catch (...)
				{
				}
			}
		}

		number->m_text = std::to_wstring(floats.back());
		while (number->m_text.back() == L'0' || number->m_text.back() == L'.')
			number->m_text.pop_back();
	}
	else
	{
		if (btn == '<')
		{
			if (number->m_text.size())
				number->m_text.pop_back();
		}
		else if (btn == 'C')
			number->m_text = L"";
		else if (btn != ' ')
			number->m_text += btn;
	}
}

std::shared_ptr<LinearLayout> createCalculator()
{
	std::shared_ptr<LinearLayout> layout = std::make_shared<LinearLayout>();
	layout->setSpacing(8);
	layout->setOrientation(LinearLayout::Orientation::VERTICAL);
	layout->m_backgroundColor = Drawer::Color(0xE0, 0xE0, 0xE0);
	std::shared_ptr<TextField> number = std::make_shared<TextField>();
	number->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	number->m_textColor = Drawer::Color(0x00, 0x00, 0x00, 0xFF * 0.87);
	number->m_textSize = 36;
	number->m_cursorPos = -1;
	number->setOnCharInput([number](wchar_t ch) {
		for (size_t i = 0; i < calcButtons.size(); ++i)
			for (size_t j = 0; j < calcButtons[i].size(); ++j)
				if (calcButtons[i][j][0] == ch)
					processKey(ch, number);
		return false;
	});
	layout->addChild(number);
	drawer->focus(number);

	for (size_t i = 0; i < calcButtons.size(); ++i)
	{
		std::shared_ptr<LinearLayout> lay = std::make_shared<LinearLayout>();
		lay->setSpacing(8);
		lay->m_backgroundColor = Drawer::Color(0xE0, 0xE0, 0xE0);
		lay->setOrientation(LinearLayout::Orientation::HORIZONTAL);

		for (size_t j = 0; j < calcButtons[i].size(); ++j)
		{
			std::shared_ptr<Button> btn = std::make_shared<Button>();
			if (calcButtons[i][j] == L" ")
			{
				btn->m_backgroundColor = Drawer::Color(0xE0, 0xE0, 0xE0);
				btn->m_pressedColor = Drawer::Color(0xE0, 0xE0, 0xE0);
			}
			else
			{
				btn->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
				btn->m_pressedColor = Drawer::Color(0xEE, 0xEE, 0xEE);
			}
			btn->m_textColor = Drawer::Color(0x00, 0x00, 0x00, 0xFF * 0.87);
			btn->m_textSize = 48;
			btn->m_text = calcButtons[i][j];
			btn->setOnMouseEvent([btn, number] (const MouseEvent &ev) {
				if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
					processKey(btn->m_text[0], number);
				return true;
			});
			lay->addChild(btn);
		}

		layout->addChild(lay);
	}

	return layout;
}

int prevFrom = -1;
void pushMessage(std::shared_ptr<LinearLayout> msgLay, std::shared_ptr<TextArea> inputText, bool asMe)
{
	std::shared_ptr<LinearLayout> curMsgHorzLay = std::make_shared<LinearLayout>(View::SizePolitics::MATCH_PARENT, View::SizePolitics::WRAP_CONTENT);
	curMsgHorzLay->setOrientation(LinearLayout::Orientation::HORIZONTAL);
	std::shared_ptr<Spacer> spacer = std::make_shared<Spacer>(200, 0);
	msgLay->addChild(curMsgHorzLay);

	std::shared_ptr<LinearLayout> curMsgLay = std::make_shared<LinearLayout>(View::SizePolitics::MATCH_PARENT, View::SizePolitics::WRAP_CONTENT);
	if (asMe) curMsgHorzLay->addChild(spacer);
	curMsgHorzLay->addChild(curMsgLay);
	if (!asMe) curMsgHorzLay->addChild(spacer);
	 
	if (prevFrom == -1 || prevFrom != asMe)
	{
		std::shared_ptr<LinearLayout> usernameLay = std::make_shared<LinearLayout>(View::SizePolitics::WRAP_CONTENT, View::SizePolitics::WRAP_CONTENT);
		usernameLay->setOrientation(LinearLayout::Orientation::HORIZONTAL);
		curMsgLay->addChild(usernameLay);
		if (asMe)
		{
			std::shared_ptr<Spacer> spacer = std::make_shared<Spacer>();
			usernameLay->addChild(spacer);
		}
		std::shared_ptr<TextField> username = std::make_shared<TextField>(View::SizePolitics::WRAP_CONTENT, View::SizePolitics::WRAP_CONTENT);
		username->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
		username->m_textColor = Drawer::Color(0x00, 0x00, 0x00);
		username->m_textSize = 24;
		username->m_paddingX = 8;
		username->m_paddingY = 8;
		username->m_text = asMe ? L"[My username]" : L"[Other username]";
		usernameLay->addChild(username);
	}
	prevFrom = asMe;

	std::shared_ptr<TextArea> msg = std::make_shared<TextArea>(View::SizePolitics::MATCH_PARENT, View::SizePolitics::WRAP_CONTENT);
	msg->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	msg->m_textColor = Drawer::Color(0x00, 0x00, 0x00);
	msg->m_textSize = 48;
	msg->m_paddingX = 8;
	msg->m_paddingY = 8;
	msg->m_text = inputText->m_text;
	curMsgLay->addChild(msg);

	inputText->m_cursorPos = 0;
	inputText->m_text = L"";
}

std::shared_ptr<LinearLayout> createChat()
{
	std::shared_ptr<LinearLayout> lay = std::make_shared<LinearLayout>();
	lay->m_backgroundColor = Drawer::Color(0xE0, 0xE0, 0xE0);
	lay->setSpacing(8);
	
	std::shared_ptr<LinearLayout> msgLay = std::make_shared<LinearLayout>();
	msgLay->setSpacing(8);
	lay->addChild(msgLay);

	std::shared_ptr<LinearLayout> inputLay = std::make_shared<LinearLayout>(View::SizePolitics::MATCH_PARENT, View::SizePolitics::WRAP_CONTENT);
	inputLay->setSpacing(8);
	inputLay->setOrientation(LinearLayout::Orientation::HORIZONTAL);
	lay->addChild(inputLay);

	std::shared_ptr<TextArea> inputText = std::make_shared<TextArea>();
	inputText->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	inputText->m_textColor = Drawer::Color(0x00, 0x00, 0x00);
	inputText->m_textSize = 48;
	inputText->m_paddingX = 8;
	inputText->m_paddingY = 8;
	inputLay->addChild(inputText);

	std::shared_ptr<Button> inputSendAsMe = std::make_shared<Button>(View::SizePolitics::WRAP_CONTENT, View::SizePolitics::MATCH_PARENT);
	inputSendAsMe->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	inputSendAsMe->m_pressedColor = Drawer::Color(0xE0, 0xE0, 0xE0);
	inputSendAsMe->m_textColor = Drawer::Color(0x00, 0x00, 0x00);
	inputSendAsMe->m_textSize = 48;
	inputSendAsMe->m_paddingX = 8;
	inputSendAsMe->m_paddingY = 8;
	inputSendAsMe->m_text = L"Send as me";
	inputSendAsMe->setOnMouseEvent([=] (const MouseEvent &ev) {
		if (ev.action == MouseEvent::Action::PRESS)
			pushMessage(msgLay, inputText, true);
		return true;
	});
	inputLay->addChild(inputSendAsMe);

	std::shared_ptr<Button> inputSendAsOther = std::make_shared<Button>(View::SizePolitics::WRAP_CONTENT, View::SizePolitics::MATCH_PARENT);
	inputSendAsOther->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	inputSendAsOther->m_pressedColor = Drawer::Color(0xE0, 0xE0, 0xE0);
	inputSendAsOther->m_textColor = Drawer::Color(0x00, 0x00, 0x00);
	inputSendAsOther->m_textSize = 48;
	inputSendAsOther->m_paddingX = 8;
	inputSendAsOther->m_paddingY = 8;
	inputSendAsOther->m_text = L"Send as other";
	inputSendAsOther->setOnMouseEvent([=] (const MouseEvent &ev) {
		if (ev.action == MouseEvent::Action::PRESS)
			pushMessage(msgLay, inputText, false);
		return true;
	});
	inputLay->addChild(inputSendAsOther);

	return lay;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	drawer->setSize(width, height);
	drawer->render();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
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

	mainLayout->onMouseEvent(MouseEvent(btn, act, x, y));
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	drawer->onCharInput(codepoint);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawer = std::make_shared<Drawer>(window, 1280, 720);
	//mainLayout = createLayout(true, 10);
	//mainLayout = createCalculator();
	mainLayout = createChat();
	drawer->setRoot(mainLayout);

	while (!glfwWindowShouldClose(window)) {
		double prevTime = glfwGetTime();
		drawer->render();
		glfwPollEvents();
		int32_t time = (1 / 60.0f - glfwGetTime() + prevTime) * 1000 * 1000;
		if (time >= 0)
			SleepInUs(time);
	}
	
	glfwTerminate();
	return 0;
}