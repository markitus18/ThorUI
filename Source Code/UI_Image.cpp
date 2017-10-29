#include "UI_Image.h"
#include "ThorUI.h"

UI_Image::UI_Image(Vec2 pos, Vec2 size, int texture_id) : UI_Item(pos, size)
{
	name = "Image";
	type = Image;
	SetTexture(texture_id);
}

void UI_Image::Draw()
{
	if (texture_id != 0)
	{
		ThorUI::DrawImage(global_pos, size, texture_id, color);
	}
	else
	{
		ThorUI::DrawQuad(global_pos, size, color, true);
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

uint UI_Image::GetTexID() const
{
	return texture_id;
}