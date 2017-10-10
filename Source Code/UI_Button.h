#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include <cstring>

#include "UI_Item.h"
#include "Color.h"

struct Button_Apparence
{
	Button_Apparence() { memset(change_attribute, false, 4); };
	Vec2 size;
	Vec2 position;
	int texture_id;
	Color color;
	bool change_attribute[4];
};


class UI_Button : public UI_Item
{
public:
	UI_Button() { current_color = color[0] = Color::White(); color[1] = Color::Blue(); color[2] = Color::Red();};
	UI_Button(Vec2 size, Vec2 pos) : UI_Item(size, pos) { current_color = color[0] = Color::Teal(); color[1] = Color::Olive(); color[2] = Color::Cyan(); };

	void Draw();
	void OnItemEvent(Item_Event event);

private:
	Color current_color;
	Color color[3];
};

#endif // !__UI_BUTTON_H__

