#include "UI_Button.h"
#include "ThorUI.h"
#include "Log.h"

UI_Button::UI_Button()
{
	color = color_data[0] = Color::White();
	color_data[1] = Color::Blue();
	color_data[2] = Color::Red();
	name = "Button";
	type = Button;
}

UI_Button::UI_Button(Vec2 size, Vec2 pos) : UI_Item(size, pos)
{
	color = color_data[0] = Color::Teal();
	color_data[1] = Color::Olive();
	color_data[2] = Color::Cyan();
	name = "Button";
	type = Button;
}

void UI_Button::Draw()
{
	ThorUI::DrawQuad(global_pos - (size * pivot), size, color, true, 5.0f);
}

void UI_Button::OnItemEvent(Item_Event event)
{
	UI_Item::OnItemEvent(event);
	switch (event)
	{
		case(Mouse_Enter):	color = color_data[1]; break;
		case(Mouse_Down):	color = color_data[2]; break;
		case(Mouse_Up):		color = color_data[1]; break;
		case(Mouse_Exit):	color = color_data[0]; break;
	}
}