#include <iostream>
#include "DrawerOpenGL.h"
#include "Button.h"
#include "LinearLayout.h"
#include "TextField.h"
#include "TextArea.h"
#include "Spacer.h"
#include "DropdownList.h"
#include "Checkbox.h"
#include "Image.h"

using namespace fastui;

std::vector<std::vector<UnicodeString>> calcButtons = {
	{ "7", "8", "9", "+", "<="},
	{ "4", "5", "6", "-", "C"},
	{ "1", "2", "3", "*", " "},
	{ ".", "0", "=", "/", " "}
};
std::unordered_map<UnicodeString::char_type, int8_t> calcPriorities = {
	{'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}
};

float prevNumber = 0;
UnicodeString prevOp = "";

void processKey(UnicodeString::char_type btn, std::shared_ptr<TextField> number)
{
	if (btn == L'=')
	{
		UnicodeString num;
		std::vector<UnicodeString> nums;
		std::vector<UnicodeString::char_type> ops;

		//RPN
		for (size_t i = 0; i < number->m_text.size(); ++i)
		{
			UnicodeString::char_type ch = number->m_text[i];
			if ((ch >= '0' && ch <= '9') || ch == '.')
				num += ch;
			else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
			{
				nums.push_back(num);
				num = "";
				if (ops.empty())
					ops.push_back(ch);
				else
				{
					while (ops.size() && calcPriorities[ch] <= calcPriorities[ops.back()])
					{
						UnicodeString str;
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
			UnicodeString str;
			str += ops.back();
			nums.push_back(str);
			ops.pop_back();
		}

		//Calc	
		std::vector<float> floats;
		for (const UnicodeString &str : nums)
		{
			if (str == "+")
			{
				floats[floats.size() - 2] += floats.back();
				floats.pop_back();
			}
			else if (str == "-")
			{
				floats[floats.size() - 2] -= floats.back();
				floats.pop_back();
			}
			else if (str == "*")
			{
				floats[floats.size() - 2] *= floats.back();
				floats.pop_back();
			}
			else if (str == "/")
			{
				floats[floats.size() - 2] /= floats.back();
				floats.pop_back();
			}
			else
			{
				try {
					floats.push_back(std::stof(str.toUtf8()));
				}
				catch (...)
				{
				}
			}
		}

		number->m_text = UnicodeString(std::to_string(floats.back()).c_str());
		if (number->m_text.find('.') != -1) {
			while (number->m_text[number->m_text.size() - 1] == '0')
				number->m_text.pop_back();
			if (number->m_text[number->m_text.size() - 1] == '.')
				number->m_text.pop_back();
		}
	}
	else
	{
		if (btn == '<')
		{
			if (number->m_text.size())
				number->m_text.pop_back();
		}
		else if (btn == 'C')
			number->m_text = "";
		else if (btn != ' ')
			number->m_text += btn;
	}
}

std::shared_ptr<LinearLayout> createCalculator(std::shared_ptr<Drawer> drawer)
{
	std::shared_ptr<LinearLayout> layout = std::make_shared<LinearLayout>();
	layout->m_paddingX = 8;
	layout->m_paddingY = 8;
	layout->setSpacing(8);
	layout->setOrientation(LinearLayout::Orientation::VERTICAL);
	layout->m_backgroundColor = Drawer::Color(0xF0, 0xF0, 0xF0);
	std::shared_ptr<TextField> number = std::make_shared<TextField>();
	number->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	number->m_textColor = Drawer::Color(0x00, 0x00, 0x00, 0xDE);
	number->m_textSize = 56;
	number->m_paddingX = 8;
	number->m_paddingY = 8;
	number->m_cursorPos = -1;
	number->setOnCharInput([number](UnicodeString::char_type ch) {
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
		lay->setOrientation(LinearLayout::Orientation::HORIZONTAL);

		for (size_t j = 0; j < calcButtons[i].size(); ++j)
		{
			if (calcButtons[i][j] == " ")
				lay->addChild(std::make_shared<Spacer>());
			else
			{
				std::shared_ptr<Button> btn = std::make_shared<Button>(View::SizePolitics::MATCH_PARENT);
				btn->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
				btn->m_textColor = Drawer::Color(0x00, 0x00, 0x00, 0xDE);
				btn->m_textSize = 24;
				btn->m_text = calcButtons[i][j];
				btn->setOnMouseEvent([btn, number](const MouseEvent& ev) {
					if (ev.action == MouseEvent::Action::PRESS && ev.button == MouseEvent::Button::LEFT)
						processKey(btn->m_text[0], number);
					return true;
					});
				lay->addChild(btn);
			}
		}

		layout->addChild(lay);
	}

	return layout;
}

int main()
{
	auto drawer = std::make_shared<DrawerOpenGL>(400, 420);
	drawer->setTitle("Calculator - FastUI");
	drawer->setRoot(createCalculator(drawer));
	drawer->execute();
	return 0;
}
