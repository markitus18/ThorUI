#ifndef __UI_ITEM_H__
#define __UI_ITEM_H__

#include "Vec2.h"

struct SDL_Texture;

class UI_Item
{

public:
	UI_Item() {};
	UI_Item(int x, int y) : pos(x, y) {};
	UI_Item(Vec2 pos, Vec2 size) : pos(pos), size(size) {};

	void SetPos(int x, int y);
	void SetSize(int w, int h);

	virtual void Draw() {}; //TODO: draw here, or in ThorUI?

protected:
	Vec2 pos;
	Vec2 size;
};

#endif
