#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "UI_Item.h"
#include "Color.h"

#include <string>

class UI_Text : public UI_Item
{
public:
	UI_Text();
	UI_Text(Vec2 pos, Vec2 size, const char* text);
	~UI_Text();

	void SetText(const char* text);
	void SetColor(const Color& color);
	void SetFont(uint font_id);
	void SetSize(Vec2 size);

	const char* GetText() const;
	Color GetColor() const;
	Vec2 GetTexSize() const;

	void Draw();

protected:
	void InternalSave(Config& config);
	void InternalLoad(Config& config);

private:
	bool LoadTexture();

private:
	Vec2 texture_size;
	std::string text;
	Color color;
	uint texture_id = 0;
	uint font_id = 1;
};

#endif // !UI_TEXT_H

