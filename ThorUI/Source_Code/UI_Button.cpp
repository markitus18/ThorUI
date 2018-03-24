#include "UI_Button.h"
#include "ThorUI.h"
#include "Log.h"
#include "Config.h"

#include "glew-2.1.0\include\GL\glew.h" //TODO: use it only in ThorUI?

UI_Button::UI_Button()
{
	color = color_data[0] = Color::Teal();
	color_data[1] = Color::Olive();
	color_data[2] = Color::Cyan();
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

	//s_clicked.connect_manager<UI_Button>(this, &UI_Button::SignalManager);
	//s_pressed.connect_manager<UI_Button>(this, &UI_Button::SignalManager);
}

void UI_Button::Draw()
{
	if (IsActiveHierarchy() == true)
	{
		glPushMatrix();
		glMultMatrixf(transform.Center().OpenGLPtr());
		
		ThorUI::DrawQuad(-size/2, size, color, true, 5.0f);

		glPopMatrix();
	}
}

void UI_Button::OnItemEvent(Item_Event event)
{
	switch (event)
	{
		case(Mouse_Enter):
		{
			color = color_data[1];
			break;
		}
		case(Mouse_Down):
		{
			color = color_data[2];
			s_pressed.Emit(5);
			break;
		}
		case(Mouse_Up):
		{
			color = color_data[1];
			s_clicked.Emit();
			break;
		}
		case(Mouse_Exit):
		{
			color = color_data[0];
			break;
		}
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

void UI_Button::Save(Config& config)
{
	config.SetArray("Color").AddColor(color);
}

void UI_Button::Load(Config& config)
{
	color = config.GetArray("Color").GetColor(0);
}

bool UI_Button::ConnectItemWithSignal(UI_Item* item, std::string signal_name)
{
	if (signal_name == "pressed")
	{
		s_pressed.connect_manager<UI_Item>(item, &UI_Item::SignalManager);
		return true;
	}
	if (signal_name == "clicked")
	{
		s_clicked.connect_manager<UI_Item>(item, &UI_Item::SignalManager);
		return true;
	}
	return UI_Item::ConnectItemWithSignal(item, signal_name);
}

std::vector<std::string> UI_Button::GetSignalStr()
{
	std::vector<std::string> ret = UI_Item::GetSignalsStr();
	ret.push_back("pressed");
	ret.push_back("clicked");
	return ret;
}