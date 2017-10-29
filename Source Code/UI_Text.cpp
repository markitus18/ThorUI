#include "UI_Text.h"
#include "SDL2_ttf-2.0.14\include\SDL_ttf.h"
#include "ThorUI.h"

UI_Text::UI_Text() : color(Color::White())
{
	name = "Text";
	type = Text;
}

UI_Text::UI_Text(Vec2 pos, const char* text) : UI_Item(pos, Vec2(-1, -1)), color(Color::White()), text(text)
{
	name = "Text";
	type = Text;
}

UI_Text::UI_Text(Vec2 pos, Vec2 size, const char* text) :  UI_Item(pos, size), color(Color::White()), text(text)
{
	name = "Text";
	type = Text;
}

UI_Text::~UI_Text()
{
	if (font_id != 0)
		ThorUI::OnLeaveFont(font_id);
}

void UI_Text::SetText(const char* text)
{
	if (this->text != text)
	{
		this->text = text;
		LoadTexture();
	}
}

void UI_Text::SetColor(const Color& color)
{
	this->color = color;
}

void UI_Text::SetFont(uint font_id)
{
	if (font_id == 0 || this->font_id == font_id)
		return;

	if (this->font_id != 0)
		ThorUI::OnLeaveFont(this->font_id);

	this->font_id = font_id;
	ThorUI::OnSetFont(font_id);
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
	ThorUI::DrawImage(global_pos, size, texture_id, color);
}

bool UI_Text::LoadTexture()
{
	if (font_id != 0 && text != "")
	{
		if (texture_id != 0)
			ThorUI::OnLeaveTexture(texture_id);

		Vec2 texture_size;
		texture_id = ThorUI::GenTextTexture(text.c_str(), font_id, texture_size);
		if (texture_id == 0) return false;

		//Adjusting text size to texture created
		if (size.x == -1 && size.y == -1)
			size = texture_size;
	}
}