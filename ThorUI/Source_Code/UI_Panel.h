#ifndef __UI_PANEL_H__
#define __UI_PANEL_H__

#include "UI_Item.h"

class UI_Panel : public UI_Item
{
public:
	THORUI_API UI_Panel() { name = "Panel"; type = Panel; };
	THORUI_API UI_Panel(Vec2 pos, Vec2 size);

	THORUI_API void Draw();
	THORUI_API void OnItemEvent(Item_Event event) {}

	THORUI_API void SetColor(Color color);
	THORUI_API void SetBorderColor(Color color);
	THORUI_API void SetBorderWidth(int width);

	THORUI_API Color GetColor() const;
	THORUI_API Color GetBorderColor() const;
	THORUI_API int GetBorderWidth() const;

	THORUI_API void Save(Config& config);
	THORUI_API void Load(Config& config);

	THORUI_API virtual void SetAppearanceSet(uint index);

public:

private:
	Color color;
	Color border_color;

	int border_width = 10.0f;
};

#endif
