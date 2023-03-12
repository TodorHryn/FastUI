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
	drawer->setTitle("Controls - FastUI");
	drawer->setRoot(createControls());
	drawer->execute();
	return 0;
}
