#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI_Item.h"
#include "Color.h"
#include <map>

class Config;

class UI_Button : public UI_Item
{
public:
	THORUI_API UI_Button();
	THORUI_API UI_Button(Vec2 size, Vec2 pos);

	THORUI_API void Draw();
	THORUI_API void OnItemEvent(Item_Event event);

	THORUI_API void SetColor(Color color);
	THORUI_API void SetTexture(uint texture_id);

	THORUI_API uint GetTexID() const;
	THORUI_API Color GetColor() const;

	THORUI_API void Save(Config& config);
	THORUI_API void Load(Config& config);

	THORUI_API virtual bool ConnectItemWithSignal(UI_Item* item, std::string signal_name, Signal_Event& s_ev);
	THORUI_API virtual bool DisconnectItemWithSignal(UI_Item* item, Signal_Event& s_ev);
	THORUI_API virtual std::vector<std::string> GetSignalsStr();

	THORUI_API virtual Appearance_Set SaveCurrentApState();
	THORUI_API virtual void UpdateApSetTransition(float dt);


public:
	//Signals
	//* Emitted when the mouse is pressed (mouse down)
	Signal<> s_pressed;
	//* Emitted when the mouse is released (and previously pressed) in the item (mouse up)
	Signal<> s_clicked;

private:
	uint texture_id = 0;
	Color color;
};

#endif // !__UI_BUTTON_H__

