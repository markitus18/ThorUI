#include "UI_Image.h"
#include "ThorUI.h"
#include "Config.h"

UI_Image::UI_Image(Vec2 pos, Vec2 size, int texture_id) : UI_Item(pos, size)
{
	name = "Image";
	type = Image;
	SetTexture(texture_id);
}

void UI_Image::Draw()
{
	if (IsParentActive() == false) return;

	if (texture_id != 0)
	{
		ThorUI::DrawImage(global_rect.pos - (rect.size * rect.pivot), rect.size * global_scale, texture_id, color);
	}
	else
	{
		ThorUI::DrawQuad(global_rect.pos - (rect.size * rect.pivot * global_scale), rect.size * global_scale, color, true);
	}
}

void UI_Image::InternalSave(Config& config)
{
	ThorUI::Texture* tex = ThorUI::GetTexture(texture_id);
	config.SetString("Texture", tex != nullptr ? tex->path.c_str() : "");
	config.SetArray("Color").AddColor(color);
}

void UI_Image::InternalLoad(Config& config)
{
	color = config.GetArray("Color").GetColor(0);
	std::string tex_path =  config.GetString("Texture");
	if (tex_path != "")
	{
		int tex_id = ThorUI::LoadTexture(tex_path.c_str());
		if (tex_id != 0)
			SetTexture(tex_id);
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
	if (rect.size.x == -1 && rect.size.y == -1)
	{
		ThorUI::Texture* tex = ThorUI::GetTexture(texture_id);
		if (tex != nullptr)
		{
			rect.size.Set(tex->original_size.x, tex->original_size.y);
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