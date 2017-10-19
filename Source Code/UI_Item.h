#ifndef __UI_ITEM_H__
#define __UI_ITEM_H__

#include "Vec2.h"

enum Item_Event
{
	Mouse_Enter,
	Mouse_Down,
	//TODO: Add Mouse_Hold?
	Mouse_Up,
	Mouse_Exit,
};

struct SDL_Texture;

class UI_Item
{
public:
	UI_Item() {};
	UI_Item(float x, float y) : pos(x, y) {};
	UI_Item(Vec2 pos, Vec2 size) : pos(pos), size(size) {};

	void SetPos(float x, float y);
	void SetSize(float w, float h);

	virtual void Draw() {}; //TODO: draw here, or in ThorUI?
	virtual void OnItemEvent(Item_Event event) { last_event = event; };

	Vec2 GetPos() const;
	Vec2 GetSize() const;
	Item_Event GetLastEvent() const;

protected:
	Vec2 pos;
	Vec2 size;
	Item_Event last_event = Mouse_Exit;
};

#endif
