#include "UI_Text.h"
#include "ThorUI.h"
#include "Config.h"

#include "glew-2.1.0\include\GL\glew.h" //TODO

UI_Text::UI_Text() : color(Color::White())
{
	name = "Text";
	type = Item_Type::Text;
}

UI_Text::UI_Text(Vec2 pos, Vec2 size, const char* text) :  UI_Item(pos, size), color(Color::White()), text(text)
{
	name = "Text";
	ThorUI::OnSetFont(font_id);
	LoadTexture();
	type = Item_Type::Text;
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

	if (font_id <= ThorUI::fonts.size())
	{
		this->font_id = font_id;
		ThorUI::OnSetFont(font_id);
		LoadTexture();
	}
	else
	{
		font_id = 0;
	}

}

void UI_Text::SetSize(Vec2 size)
{
	this->size = size;
	transform.SetPivot((Vec2(0.5f, 0.5f) - pivot) * size);
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

Vec2 UI_Text::GetTexSize() const
{
	return texture_size;
}

Appearance_Set UI_Text::SaveCurrentApState()
{
	Appearance_Set set = UI_Item::SaveCurrentApState();

	set.text_ap = new Text_Ap();
	set.text_ap->SetAllAttributesTrue();
	set.text_ap->color = color;
	set.text_ap->text = text;

	return set;
}

void UI_Text::UpdateApSetTransition(float dt)
{
	UI_Item::UpdateApSetTransition(dt);

	Appearance_Set& set = appearance_sets[current_set_index];
	if (set.text_ap != nullptr)
	{
		if (set.text_ap->attributes["color"] == true)
			color = set.text_ap->color;
		if (set.text_ap->attributes["text"] == true)
			SetText(set.text_ap->text.c_str());
	}
}
void UI_Text::Draw()
{
	if (IsActiveHierarchy() == true && texture_id != 0)
	{
		glPushMatrix();
		glMultMatrixf(transform.Center().OpenGLPtr());

		ThorUI::DrawImage(-size/2, size, texture_id, color);

		glPopMatrix();
	}
}

void UI_Text::Save(Config& config)
{
	config.SetString("Text", text.c_str());

	ThorUI::Font* font = &ThorUI::fonts[font_id - 1];
	if (font != nullptr)
	{
		config.SetString("Font", font->path.c_str());
		config.SetNumber("Font Size", font->size);
	}

	config.SetArray("Color").AddColor(color);
}

void UI_Text::Load(Config& config)
{
	text = config.GetString("Text", "Undefined");

	std::string font = config.GetString("Font");
	int font_size = config.GetNumber("Font Size");
	if (font != "")
	{
		font_id = ThorUI::LoadFont(font.c_str(), font_size);
	}

	color = config.GetArray("Color").GetColor(0);
	LoadTexture();
}

bool UI_Text::LoadTexture()
{
	if (texture_id != 0)
		ThorUI::FreeTexture(texture_id);

	if (font_id != 0 && text != "")
	{
		texture_id = ThorUI::GenTextTexture(text.c_str(), font_id, texture_size, size);
		size = texture_size;
	}
	else
	{
		texture_id = 0;
	}
	return texture_id != 0;
}