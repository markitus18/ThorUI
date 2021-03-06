#include "UI_Image.h"
#include "ThorUI.h"
#include "Config.h"

#include "glew-2.1.0\include\GL\glew.h" //TODO

UI_Image::UI_Image(Vec2 pos, Vec2 size, int texture_id) : UI_Item(pos, size)
{
	name = "Image";
	type = Item_Type::Image;
	SetTexture(texture_id);
}

void UI_Image::Draw()
{
	if (IsActiveHierarchy() == false) return;

	glPushMatrix();
	glMultMatrixf(transform.Center().OpenGLPtr());

	if (texture_id != 0)
	{
		ThorUI::DrawImage(-size/2, size, texture_id, color);
	}
	else
	{
		ThorUI::DrawQuad(-size/2, size, color, true);
	}

	glPopMatrix();
}

void UI_Image::Save(Config& config)
{
	ThorUI::Texture* tex = ThorUI::GetTexture(texture_id);
	config.SetString("Texture", tex != nullptr ? tex->path.c_str() : "");
	config.SetArray("Color").AddColor(color);
}

void UI_Image::Load(Config& config)
{
	std::string tex_path =  config.GetString("Texture");
	if (tex_path != "")
	{
		int tex_id = ThorUI::LoadTexture(tex_path.c_str());
		if (tex_id != 0)
			SetTexture(tex_id);
	}
	color = config.GetArray("Color").GetColor(0);
}

Appearance_Set UI_Image::SaveCurrentApState()
{
	Appearance_Set set = UI_Item::SaveCurrentApState();

	set.image_ap = new Image_Ap();
	set.image_ap->SetAllAttributesTrue();
	set.image_ap->color = color;
	set.image_ap->texture_id = texture_id;

	return set;
}

void UI_Image::UpdateApSetTransition(float dt)
{
	UI_Item::UpdateApSetTransition(dt);
	float lerpRatio = transition_timer / transition_max_timer;

	Appearance_Set& set = appearance_sets[current_set_index];
	if (set.image_ap != nullptr)
	{
		if (set.image_ap->attributes["color"] == true)
			color = Color::Lerp(previous_set.image_ap->color, set.image_ap->color, lerpRatio);
		if (set.image_ap->attributes["texture"] == true)
			SetTexture(set.image_ap->texture_id);
	}
}
void UI_Image::SetColor(Color color)
{
	this->color = color;
}

void UI_Image::SetTexture(uint texture_id)
{
	if (texture_id == 0 || this->texture_id == texture_id) return;

	if (this->texture_id != 0)
		ThorUI::OnLeaveTexture(this->texture_id);

	this->texture_id = texture_id;
	ThorUI::OnSetTexture(texture_id);

	//Adjust size to texture size
	if (size.x == -1 && size.y == -1)
	{
		ThorUI::Texture* tex = ThorUI::GetTexture(texture_id);
		if (tex != nullptr)
		{
			size.Set(tex->original_size.x, tex->original_size.y);
		}
	}
}

Color UI_Image::GetColor() const
{
	return color;
}

uint UI_Image::GetTexID() const
{
	return texture_id;
}