#include "UIImage.h"

UI_Image::UI_Image(Vec2 pos, Vec2 size, int texture_id) : UI_Item(pos, size), texture_id(texture_id)
{

}

void UI_Image::Draw()
{
	//Call openGL texture and quad
}