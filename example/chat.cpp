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
	inputSendAsMe->m_textSize = 24;
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
	inputSendAsOther->m_textSize = 24;
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

int main()
{
	auto drawer = std::make_shared<DrawerOpenGL>(1280, 720);
	drawer->setTitle("Chat - FastUI");
	drawer->setRoot(createChat());
	drawer->execute();
	return 0;
}
