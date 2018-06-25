#include "UI_Panel.h"

#include "Color.h";
#include "ThorUI.h"
#include "Config.h"
#include "glew-2.1.0\include\GL\glew.h" //TODO

UI_Panel::UI_Panel(Vec2 pos, Vec2 size) : UI_Item(pos, size)
{
	name = "Panel";
	type = Item_Type::Panel;
	border_color = Color::Black();
	color = Color::Cyan();
	color.a = 0.3f;
	this->size = Vec2(500, 300);
}

void UI_Panel::Draw()
{
	if (IsActiveHierarchy() == false) return;

	glPushMatrix();
	glMultMatrixf(transform.Center().OpenGLPtr());

	ThorUI::DrawQuad((-size / 2), size, border_color, false, border_width);
	ThorUI::DrawQuad((-size / 2) + Vec2(border_width, border_width), size - (Vec2(border_width, border_width) * 2), color, true);

	glPopMatrix();
}

void UI_Panel::SetColor(Color color)
{
	this->color = color;
}

void UI_Panel::SetBorderColor(Color color)
{
	border_color = color;
}

void UI_Panel::SetBorderWidth(int width)
{
	border_width = width;
}

Color UI_Panel::GetColor() const
{
	return color;
}

Color UI_Panel::GetBorderColor() const
{
	return border_color;
}

int UI_Panel::GetBorderWidth() const
{
	return border_width;
}

void UI_Panel::Save(Config& config)
{
	config.SetArray("Color").AddColor(color);
	config.SetArray("Border_Color").AddColor(border_color);
	config.SetNumber("Border_Width", border_width);
}

void UI_Panel::Load(Config& config)
{
	color = config.GetArray("Color").GetColor(0);
	border_color = config.GetArray("Border_Color").GetColor(0);
	border_width = config.GetNumber("Border_Width");
}

Appearance_Set UI_Panel::SaveCurrentApState()
{
	Appearance_Set set = UI_Item::SaveCurrentApState();

	set.panel_ap = new Panel_Ap();
	set.panel_ap->SetAllAttributesTrue();
	set.panel_ap->color = color;
	set.panel_ap->border_color = border_color;
	set.panel_ap->border_width = border_width;

	return set;
}

void UI_Panel::UpdateApSetTransition(float dt)
{
	UI_Item::UpdateApSetTransition(dt);
	float lerpRatio = transition_timer / transition_max_timer;

	Appearance_Set& set = appearance_sets[current_set_index];
	if (set.panel_ap != nullptr)
	{
		if (set.panel_ap->attributes["color"] == true)
			color = Color::Lerp(previous_set.panel_ap->color, set.panel_ap->color, lerpRatio);
		if (set.panel_ap->attributes["border_color"] == true)
			border_color = Color::Lerp(previous_set.panel_ap->border_color, set.panel_ap->border_color, lerpRatio);
		if (set.panel_ap->attributes["border_width"] == true)
			border_width = Math::Lerp(previous_set.panel_ap->border_width, set.panel_ap->border_width, lerpRatio);
	}
}