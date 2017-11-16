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
	UI_Button();
	UI_Button(Vec2 size, Vec2 pos);

	void Draw();
	void OnItemEvent(Item_Event event);

	void SetColor(Color color);

	Color GetColor() const;

	void InternalSave(Config& config);
	void InternalLoad(Config& config);

private:
	Color color;
	Color color_data[3];
};

#endif // !__UI_BUTTON_H__

