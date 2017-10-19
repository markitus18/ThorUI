#include "UI_Text.h"
#include "SDL2_ttf-2.0.14\include\SDL_ttf.h"
#include "ThorUI.h"

UI_Text::UI_Text() : color(Color::White())
{

}

void UI_Text::SetText(const char* text)
{
	this->text = text;
	LoadTexture();
}

void UI_Text::SetColor(const Color& color)
{
	this->color = color;
}

void UI_Text::SetFont(uint font_id)
{
	this->font_id = font_id;
	LoadTexture();
}

const char* UI_Text::GetText() const
{
	return text.c_str();
}

Color UI_Text::GetColor() const
{
	return color;
}

void UI_Text::Draw()
{
	ThorUI::DrawImage(pos, size, texture_id, color);
}

void UI_Text::LoadTexture()
{
	if (font_id != 0 && text != "")
	{
		texture_id = ThorUI::LoadTextTexture(text.c_str(), font_id, color);
		texture_created = true;
	}
}