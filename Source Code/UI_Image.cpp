#include "UI_Image.h"
#include "ThorUI.h"

UI_Image::UI_Image(Vec2 pos, Vec2 size, int texture_id) : UI_Item(pos, size), texture_id(texture_id)
{
	if (texture_id != 0)
		ThorUI::
}

void UI_Image::Draw()
{
	ThorUI::DrawImage(pos, size, texture_id, color);
}

void UI_Image::SetColor(Color color)
{
	this->color = color;
}