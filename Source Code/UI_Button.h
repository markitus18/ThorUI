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

class Config;

class UI_Button : public UI_Item
{
public:
	UI_Button() { color = color_data[0] = Color::White(); color_data[1] = Color::Blue(); color_data[2] = Color::Red(); 	name = "Text"; 	type = Button;};
	UI_Button(Vec2 size, Vec2 pos) : UI_Item(size, pos) { color = color_data[0] = Color::Teal(); color_data[1] = Color::Olive(); color_data[2] = Color::Cyan(); };

	void Draw();
	void OnItemEvent(Item_Event event);

	//void Save(Config& config);
	//void Load(Config& config);

private:
	Color color;
	Color color_data[3];
};

#endif // !__UI_BUTTON_H__

