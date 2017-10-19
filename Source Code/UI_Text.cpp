#include "UI_Text.h"
#include "SDL2_ttf-2.0.14\include\SDL_ttf.h"

UI_Text::UI_Text() : color(Color::White())
{

}

void UI_Text::SetText(const char* text)
{
	this->text = text;
}

void UI_Text::SetColor(const Color& color)
{
	this->color = color;
}

const char* UI_Text::GetText() const
{
	return text.c_str();
}

Color UI_Text::GetColor() const
{
	return color;
}