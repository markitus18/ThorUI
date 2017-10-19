#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "UI_Item.h"
#include "Color.h"

#include <string>

typedef uint;

class UI_Text : public UI_Item
{
public:
	UI_Text();
	~UI_Text() {};

	void SetText(const char* text);
	void SetColor(const Color& color);

	const char* GetText() const;
	Color GetColor() const;

private:
	std::string text;
	Color color;
	uint texture_id;

};

#endif // !UI_TEXT_H

