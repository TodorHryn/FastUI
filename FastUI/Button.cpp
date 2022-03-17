#include "Button.h"

Button::Button(SizePolitics width, SizePolitics height) 
	: View(width, height)
{
}

Button::~Button()
{
}

void Button::draw(Drawer &drawer, int32_t width, int32_t height)
{
	Drawer::State state = drawer.state();
	drawer.drawRectange(0, 0, width, height, m_backgroundColor);

	std::wstring text = L"Hi!ЪыЪ,^ĘŁ\n\nLorem ipsum dolor sit amet, ei omnis postea putant his, ea ius vitae partem everti, eum no mollis discere. Purto possit iracundia at pri, quando feugiat volumus pro te. Eos vidit alterum consequat an. Ipsum dolorem lobortis at per. Vim ignota causae platonem ad.\n\nIn mutat movet vivendum sed. Per omnes disputationi te, wisi omnium oblique at sed, ex ponderum delicata persequeris duo. Nec ut zril civibus, id pertinax posidonium quo. Eius perpetua torquatos quo no, nam cu veniam maiorum.\n\nNe minim ignota mei, graecis adolescens deterruisset te pro. Mei ne libris inermis perpetua, cu vim quem graeci indoctum. No usu case reque inani, quo oratio integre et. Pri commune eleifend dignissim in. Dolor essent no qui, sed labitur docendi consectetuer et, vim diam prodesset vituperata in.\n\nNo quo epicurei periculis. Laboramus expetendis usu ne, labore ocurreret eos ne. His ad alia sadipscing, discere impedit reformidans ne duo. Est ei velit munere omnium. Sit ea percipit interesset.\n\nCum essent animal integre in, has nullam regione voluptaria ne, usu erant impetus consetetur ea. Cu pro corpora periculis referrentur, admodum nominati at pri, ad sed quando efficiendi. Purto adipiscing sea ei, agam ferri omnis eum ne. At per quidam omnesque, ex vidit platonem pericula qui. Pro zril possim id, ea sed partem nostrum evertitur.\n\nCommodo dolorum per ei, in populo aliquam mei. Quod odio ferri mel ea, ut erat pertinax instructior vix. Magna ubique facilisi duo cu. Bonorum pertinacia ut ius.";
	auto sz = drawer.measureText(24, text);
	drawer.translate(0, height - sz.second);
	drawer.drawText(0, 0, width, height, 24, Drawer::Color(0xFF, 0xFF, 0xFF), text);

	drawer.setState(state);
}
