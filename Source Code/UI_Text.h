#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "UI_Item.h"
#include "Color.h"

#include <string>

typedef unsigned int uint;

class UI_Text : public UI_Item
{
public:
	UI_Text();
	~UI_Text() {};

	void SetText(const char* text);
	void SetColor(const Color& color);
	void SetFont(uint font_id);

	const char* GetText() const;
	Color GetColor() const;

	void Draw();

private:
	void LoadTexture();

private:
	std::string text;
	Color color;
	uint texture_id = 0;
	uint font_id = 0;

	bool texture_created = false;

};

#endif // !UI_TEXT_H

