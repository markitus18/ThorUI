#include "UI_Button.h"
#include "ThorUI.h"
#include "Log.h"
#include "Config.h"

#include "glew-2.1.0\include\GL\glew.h" //TODO: use it only in ThorUI?

UI_Button::UI_Button()
{
	color = Color::Teal();
	name = "Button";
	type = Item_Type::Button;
}

UI_Button::UI_Button(Vec2 pos, Vec2 size) : UI_Item(pos, size)
{
	color = Color::Teal();
	name = "Button";
	type = Item_Type::Button;
}

void UI_Button::Draw()
{
	if (IsActiveHierarchy() == false) return;

	glPushMatrix();
	glMultMatrixf(transform.Center().OpenGLPtr());
	
	if (texture_id != 0)
	{
		ThorUI::DrawImage(-size / 2, size, texture_id, color);
	}
	else
	{
		ThorUI::DrawQuad(-size/2, size, color, true, 5.0f);
	}

	glPopMatrix();
}

void UI_Button::OnItemEvent(Item_Event event)
{
	switch (event)
	{
		case(Mouse_Enter):
		{
			break;
		}
		case(Mouse_Down):
		{
			s_pressed.Emit();
			break;
		}
		case(Mouse_Up):
		{
			s_clicked.Emit();
			break;
		}
		case(Mouse_Exit):
		{
			break;
		}
	}
}

void UI_Button::SetColor(Color color)
{
	this->color = color;
}

void UI_Button::SetTexture(uint texture_id)
{
	if (texture_id == 0 || this->texture_id == texture_id) return;

	if (this->texture_id != 0)
		ThorUI::OnLeaveTexture(this->texture_id);

	this->texture_id = texture_id;
	ThorUI::OnSetTexture(texture_id);
}

uint UI_Button::GetTexID() const
{
	return texture_id;
}

Color UI_Button::GetColor() const
{
	return color;
}

void UI_Button::Save(Config& config)
{
	ThorUI::Texture* tex = ThorUI::GetTexture(texture_id);
	config.SetString("Texture", tex != nullptr ? tex->path.c_str() : "");
	config.SetArray("Color").AddColor(color);
}

void UI_Button::Load(Config& config)
{
	std::string tex_path = config.GetString("Texture");
	if (tex_path != "")
	{
		int tex_id = ThorUI::LoadTexture(tex_path.c_str());
		if (tex_id != 0)
			SetTexture(tex_id);
	}
	color = config.GetArray("Color").GetColor(0);
}

bool UI_Button::ConnectItemWithSignal(UI_Item* item, std::string signal_name, Signal_Event& s_ev)
{
	if (signal_name == "pressed")
	{
		s_ev.slot_id = s_pressed.connect_manager<UI_Item>(item, &UI_Item::SignalManager, item->GetID());
		s_ev.signal_id = s_pressed.GetID();
		s_ev.SetValueTypes(std::vector<std::string>{"int"});
		s_ev.signal_name = signal_name;
		return true;
	}
	if (signal_name == "clicked")
	{
		s_ev.slot_id = s_clicked.connect_manager<UI_Item>(item, &UI_Item::SignalManager, item->GetID());
		s_ev.signal_id = s_clicked.GetID();
		s_ev.SetValueTypes(std::vector<std::string>());
		s_ev.signal_name = signal_name;
		return true;
	}
	return UI_Item::ConnectItemWithSignal(item, signal_name, s_ev);
}

bool UI_Button::DisconnectItemWithSignal(UI_Item* item, Signal_Event& s_ev)
{
	if (s_ev.signal_name == "pressed")
	{
		s_pressed.disconnect_manager(s_ev.slot_id);
		s_ev.ClearSignal();
		return true;
	}
	if (s_ev.signal_name == "clicked")
	{
		s_clicked.disconnect_manager(s_ev.slot_id);
		s_ev.ClearSignal();
		return true;
	}
	return UI_Item::DisconnectItemWithSignal(item, s_ev);
}

std::vector<std::string> UI_Button::GetSignalsStr()
{
	std::vector<std::string> ret = UI_Item::GetSignalsStr();
	ret.push_back("pressed");
	ret.push_back("clicked");
	return ret;
}

Appearance_Set UI_Button::SaveCurrentApState()
{
	Appearance_Set set = UI_Item::SaveCurrentApState();

	set.button_ap = new Button_Ap();
	set.button_ap->SetAllAttributesTrue();
	set.button_ap->color = color;
	set.button_ap->texture_id = texture_id;

	return set;
}

void UI_Button::UpdateApSetTransition(float dt)
{
	UI_Item::UpdateApSetTransition(dt);
	float lerpRatio = transition_timer / transition_max_timer;

	Appearance_Set& set = appearance_sets[current_set_index];
	if (set.button_ap != nullptr)
	{
		if (set.button_ap->attributes["color"] == true)
			color = Color::Lerp(previous_set.button_ap->color, set.button_ap->color, lerpRatio);
		if (set.button_ap->attributes["texture"] == true)
			SetTexture(set.button_ap->texture_id);
	}
}