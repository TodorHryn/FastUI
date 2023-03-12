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

std::shared_ptr<LinearLayout> createLayout(bool horz, int recDepth,	int dir = 0)
{
	std::shared_ptr<LinearLayout> layout = std::make_shared<LinearLayout>();
	if (horz)
		layout->setOrientation(LinearLayout::Orientation::HORIZONTAL);
	layout->setSpacing(2);

	std::shared_ptr<Button> btn = std::make_shared<Button>();
	btn->m_textSize = 24;
	btn->m_text = "Жми сюды!";
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
				btn->m_textSize = 48;
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
	 
	if (prevFrom == -1 || static_cast<bool>(prevFrom) != asMe)
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
		username->m_textSize = 24;
		username->m_paddingX = 8;
		username->m_paddingY = 8;
		username->m_text = asMe ? "[My username]" : "[Other username]";
		username->m_editable = false;
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
	msg->m_editable = false;
	curMsgLay->addChild(msg);

	inputText->m_cursorPos = 0;
	inputText->m_text = "";
}

std::shared_ptr<LinearLayout> createChat()
{
	std::shared_ptr<LinearLayout> lay = std::make_shared<LinearLayout>();
	lay->m_backgroundColor = Drawer::Color(0xE0, 0xE0, 0xE0);
	
	std::shared_ptr<LinearLayout> msgLay = std::make_shared<LinearLayout>();
	msgLay->setSpacing(8);
	msgLay->m_paddingX = 8;
	msgLay->m_paddingY = 8;
	lay->addChild(msgLay);

	std::shared_ptr<LinearLayout> inputLay = std::make_shared<LinearLayout>(View::SizePolitics::MATCH_PARENT, View::SizePolitics::WRAP_CONTENT);
	inputLay->setSpacing(8);
	inputLay->m_backgroundColor = Drawer::Color(0xC0, 0xC0, 0xC0);
	inputLay->m_paddingX = 8;
	inputLay->m_paddingY = 8;
	inputLay->setOrientation(LinearLayout::Orientation::HORIZONTAL);
	lay->addChild(inputLay);

	std::shared_ptr<TextArea> inputText = std::make_shared<TextArea>();
	inputText->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	inputText->m_textSize = 48;
	inputText->m_paddingX = 8;
	inputText->m_paddingY = 8;
	inputLay->addChild(inputText);

	std::shared_ptr<Button> inputSendAsMe = std::make_shared<Button>();
	inputSendAsMe->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	inputSendAsMe->m_textSize = 48;
	inputSendAsMe->m_paddingX = 8;
	inputSendAsMe->m_paddingY = 8;
	inputSendAsMe->m_text = "Send as me";
	inputSendAsMe->setOnMouseEvent([=] (const MouseEvent &ev) {
		if (ev.action == MouseEvent::Action::PRESS)
			pushMessage(msgLay, inputText, true);
		return true;
	});
	inputLay->addChild(inputSendAsMe);

	std::shared_ptr<Button> inputSendAsOther = std::make_shared<Button>();
	inputSendAsOther->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	inputSendAsOther->m_textSize = 48;
	inputSendAsOther->m_paddingX = 8;
	inputSendAsOther->m_paddingY = 8;
	inputSendAsOther->m_text = "Send as other";
	inputSendAsOther->setOnMouseEvent([=] (const MouseEvent &ev) {
		if (ev.action == MouseEvent::Action::PRESS)
			pushMessage(msgLay, inputText, false);
		return true;
	});
	inputLay->addChild(inputSendAsOther);

	return lay;
}

std::shared_ptr<LinearLayout> createControls()
{
	std::shared_ptr<LinearLayout> lay = std::make_shared<LinearLayout>();
	lay->setOrientation(LinearLayout::Orientation::HORIZONTAL);
	lay->m_backgroundColor = Drawer::Color(0xF0, 0xF0, 0xF0);

	std::shared_ptr<LinearLayout> layContent0 = std::make_shared<LinearLayout>(View::SizePolitics::WRAP_CONTENT);
	layContent0->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);

	auto spWhite = std::make_shared<Spacer>();
	spWhite->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	lay->addChild(std::make_shared<Spacer>());
	lay->addChild(std::make_shared<Spacer>(*spWhite));
	lay->addChild(layContent0);
	lay->addChild(std::make_shared<Spacer>(*spWhite));
	lay->addChild(std::make_shared<Spacer>());

	std::shared_ptr<LinearLayout> layContent = std::make_shared<LinearLayout>(View::SizePolitics::WRAP_CONTENT, View::SizePolitics::WRAP_CONTENT);
	layContent->setSpacing(8);
	layContent->m_paddingY = 8;

	layContent0->addChild(std::make_shared<Spacer>());
	layContent0->addChild(layContent);
	layContent0->addChild(std::make_shared<Spacer>());

	std::shared_ptr<TextField> textField = std::make_shared<TextField>();
	textField->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	textField->m_textSize = 48;
	textField->m_paddingX = 8;
	textField->m_paddingY = 8;
	textField->m_text = "TextField 📝";
	layContent->addChild(textField);

	std::shared_ptr<TextArea> textArea = std::make_shared<TextArea>();
	textArea->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	textArea->m_textSize = 48;
	textArea->m_paddingX = 8;
	textArea->m_paddingY = 8;
	textArea->m_text = "Text\nArea\n📜✏";
	layContent->addChild(textArea);

	std::shared_ptr<DropdownList> dropdownList = std::make_shared<DropdownList>();
	dropdownList->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	dropdownList->m_textSize = 48;
	dropdownList->m_paddingX = 8;
	dropdownList->m_paddingY = 8;
	std::vector<UnicodeString> list = { "Dropdown list ⏲", "List item 🟢", "This is item number 2" };
	dropdownList->setList(list);
	layContent->addChild(dropdownList);

	std::shared_ptr<Button> btn = std::make_shared<Button>();
	btn->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	btn->m_textSize = 48;
	btn->m_paddingX = 8;
	btn->m_paddingY = 8;
	btn->m_text = "Button";
	layContent->addChild(btn);

	std::shared_ptr<Checkbox> chk = std::make_shared<Checkbox>();
	chk->m_backgroundColor = Drawer::Color(0xFF, 0xFF, 0xFF);
	chk->m_textSize = 48;
	chk->m_checkboxSize = 48;
	chk->m_checkboxBorderSize = 4;
	chk->m_paddingX = 8;
	chk->m_paddingY = 8;
	chk->m_text = "Checkbox";
	layContent->addChild(chk);

	std::shared_ptr<Image> img = std::make_shared<Image>();
	img->m_image.load("cat.jpg");
	layContent->addChild(img);

	return lay;
}

int main()
{
	auto drawer = std::make_shared<DrawerOpenGL>(1280, 720);
	//drawer->setRoot(createLayout(true, 10));
	//drawer->setRoot(createCalculator(drawer));
	//drawer->setRoot(createChat());
	drawer->setRoot(createControls());
	drawer->execute();
	return 0;
}
