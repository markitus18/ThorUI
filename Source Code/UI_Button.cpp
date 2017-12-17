#include "UI_Button.h"
#include "ThorUI.h"
#include "Log.h"
#include "Config.h"

UI_Button::UI_Button()
{
	color = color_data[0] = Color::White();
	color_data[1] = Color::Blue();
	color_data[2] = Color::Red();
	name = "Button";
	type = Button;
}

UI_Button::UI_Button(Vec2 pos, Vec2 size) : UI_Item(pos, size)
{
	color = color_data[0] = Color::Teal();
	color_data[1] = Color::Olive();
	color_data[2] = Color::Cyan();
	name = "Button";
	type = Button;
}

void UI_Button::Draw()
{
	if (IsParentActive() == true)
		ThorUI::DrawQuad(global_pos - (rect.size * pivot * global_scale), rect.size * global_scale, color, true, 5.0f);
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

void UI_Button::SetColor(Color color)
{
	color_data[0] = this->color = color;
}

Color UI_Button::GetColor() const
{
	return color;
}

void UI_Button::InternalSave(Config& config)
{
	config.SetArray("Color").AddColor(color);
}

void UI_Button::InternalLoad(Config& config)
{
	color = config.GetArray("Color").GetColor(0);
}