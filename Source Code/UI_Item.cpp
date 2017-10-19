#include "UI_Item.h"

void UI_Item::SetPos(float x, float y)
{
	pos.Set(x, y);
}

void UI_Item::SetSize(float w, float h)
{
	size.Set(w, h);
}

Vec2 UI_Item::GetPos() const
{
	return pos;
}

Vec2 UI_Item::GetSize() const
{
	return size;
}

Item_Event UI_Item::GetLastEvent() const
{
	return last_event;
}