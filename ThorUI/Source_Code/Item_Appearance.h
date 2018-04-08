#ifndef __ITEM_APPEARANCE_H__
#define __ITEM_APPEARANCE_H__

#include "Transform.h" //Vec2
#include "Color.h"
#include <string>
#include <unordered_map>

typedef unsigned int uint;
class Config;

struct Item_Ap
{
	Item_Ap()
	{
		attributes["position"] = false;
		attributes["size"] = false;
		attributes["scale"] = false;
		attributes["angle"] = false;
		attributes["pivot"] = false;
	}

	std::unordered_map<std::string, bool> attributes;

	//* Item size in pixels
	Vec2 size;
	//* Item pivot is saved as a % pivot for its rect size
	//* Range [0, 1], 0 being bottom-left, 1 top-right
	Vec2 pivot;
	//* Holds the transformation matrix
	Transform transform;

	THORUI_API void Save(Config& config);
	THORUI_API void Load(Config& config);
};

struct Image_Ap
{
	Image_Ap()
	{
		attributes["texture"] = false;
		attributes["color"] = false;
	}

	std::unordered_map<std::string, bool> attributes;

	uint texture_id;
	Color color;
};

//Same data as image, keeping it separate by now
struct Button_Ap
{
	Button_Ap()
	{
		attributes["texture"] = false;
		attributes["color"] = false;
	}

	std::unordered_map<std::string, bool> attributes;

	uint texture_id;
	Color color;
};

struct Text_Ap
{
	Text_Ap()
	{
		attributes["text"] = false;
		attributes["color"] = false;
	}

	std::unordered_map<std::string, bool> attributes;

	Vec2 texture_size;
	std::string text;
	Color color;
	uint texture_id = 0;
	uint font_id = 1;
};

struct Appearance_Set
{
	std::string name = "";

	Item_Ap* item_ap = nullptr;
	Image_Ap* image_ap = nullptr;
	Button_Ap* button_ap = nullptr;
	Text_Ap* text_ap = nullptr;

	THORUI_API void Save(Config& config);
	THORUI_API void Load(Config& config);
};
#endif