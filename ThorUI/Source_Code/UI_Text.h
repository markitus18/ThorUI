#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "UI_Item.h"
#include "Color.h"

#include <string>

class UI_Text : public UI_Item
{
public:
	THORUI_API UI_Text();
	THORUI_API UI_Text(Vec2 pos, Vec2 size, const char* text);
	THORUI_API ~UI_Text();

	THORUI_API void SetText(const char* text);
	THORUI_API void SetColor(const Color& color);
	THORUI_API void SetFont(uint font_id);
	THORUI_API void SetSize(Vec2 size);

	THORUI_API const char* GetText() const;
	THORUI_API Color GetColor() const;
	THORUI_API Vec2 GetTexSize() const;

	THORUI_API void Draw();
	THORUI_API void OnItemEvent(Item_Event event) {}

protected:
	THORUI_API void Save(Config& config);
	THORUI_API void Load(Config& config);

private:
	THORUI_API bool LoadTexture();

private:
	Vec2 texture_size;
	std::string text;
	Color color;
	uint texture_id = 0;
	uint font_id = 1;
};

#endif // !UI_TEXT_H

