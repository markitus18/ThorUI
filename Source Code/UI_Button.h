#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI_Item.h"
#include "Color.h"

class UI_Button : public UI_Item
{
public:
	UI_Button() { current_color = color[0] = Color::White(); color[1] = Color::Blue(); color[2] = Color::Red();};
	UI_Button(Vec2 size, Vec2 pos) : UI_Item(size, pos) { current_color = color[0] = Color::White(); color[1] = Color::Blue(); color[2] = Color::Red(); };

	void Draw();
	void OnItemEvent(Item_Event event);

private:
	Color current_color;
	Color color[3];
};

#endif // !__UI_BUTTON_H__

