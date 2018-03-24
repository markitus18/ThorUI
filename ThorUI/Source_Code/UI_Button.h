#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI_Item.h"
#include "Color.h"
#include <map>

struct Button_Apparence
{
	THORUI_API Button_Apparence() { memset(change_attribute, false, 4); };
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
	THORUI_API UI_Button();
	THORUI_API UI_Button(Vec2 size, Vec2 pos);

	THORUI_API void Draw();
	THORUI_API void OnItemEvent(Item_Event event);

	THORUI_API void SetColor(Color color);

	THORUI_API Color GetColor() const;

	THORUI_API void Save(Config& config);
	THORUI_API void Load(Config& config);

	THORUI_API virtual bool ConnectItemWithSignal(UI_Item* item, std::string signal_name);
	THORUI_API virtual std::vector<std::string> GetSignalStr();
	//Signals
	//* Emitted when the mouse is pressed (mouse down)
	Signal<int> s_pressed;
	//* Emitted when the mouse is released (and previously pressed) in the item (mouse up)
	Signal<> s_clicked;

private:
	Color color;
	Color color_data[3];
};

#endif // !__UI_BUTTON_H__

