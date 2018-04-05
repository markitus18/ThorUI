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
			s_pressed.Emit(5, "hello", 1.5f);
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

bool UI_Button::ConnectItemWithSignal(UI_Item* item, std::string signal_name, Signal_Event& s_ev)
{
	if (signal_name == "pressed")
	{
		s_pressed.connect_manager<UI_Item>(item, &UI_Item::SignalManager, item->GetID());
		s_ev.signal_id = s_pressed.GetID();
		s_ev.SetValueTypes(std::vector<std::string>{"int", "string", "float"});
		s_ev.signal_name = signal_name;
		return true;
	}
	if (signal_name == "clicked")
	{
		s_clicked.connect_manager<UI_Item>(item, &UI_Item::SignalManager, item->GetID());
		s_ev.signal_id = s_clicked.GetID();
		s_ev.signal_name = signal_name;
		return true;
	}
	return UI_Item::ConnectItemWithSignal(item, signal_name, s_ev);
}

std::vector<std::string> UI_Button::GetSignalsStr()
{
	std::vector<std::string> ret = UI_Item::GetSignalsStr();
	ret.push_back("pressed");
	ret.push_back("clicked");
	return ret;
}