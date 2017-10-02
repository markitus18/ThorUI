#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "UI_Item.h"

class UI_Image : public UI_Item
{
public:
	UI_Image() {};
	UI_Image(Vec2 pos, Vec2 size, int texture_id = 0);

	void Draw();

public:
	int texture_id = 0;
};

#endif // !__UIIMAGE_H__

