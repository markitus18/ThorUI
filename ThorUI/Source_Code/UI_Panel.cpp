#include "UI_Panel.h"

#include "Color.h";
#include "ThorUI.h"
#include "Config.h"
#include "glew-2.1.0\include\GL\glew.h" //TODO

UI_Panel::UI_Panel(Vec2 pos, Vec2 size) : UI_Item(pos, size)
{
	name = "Panel";
	type = Panel;
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

void UI_Panel::SetAppearanceSet(uint index)
{
	UI_Item::SetAppearanceSet(index);

	if (index < appearance_sets.size())
	{
		Appearance_Set& set = appearance_sets[index];
		if (set.panel_ap != nullptr)
		{
			if (set.panel_ap->attributes["color"] == true)
				color = set.panel_ap->color;
			if (set.panel_ap->attributes["border_color"] == true)
				border_color = set.panel_ap->border_color;
			if (set.panel_ap->attributes["border_width"] == true)
				border_width = set.panel_ap->border_width;
		}
	}
}