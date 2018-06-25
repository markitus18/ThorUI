#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

#include "UI_Item.h"

class Config;

class UI_Image : public UI_Item
{
public:
	THORUI_API UI_Image() { name = "Image"; type = Item_Type::Image; };
	THORUI_API UI_Image(Vec2 pos, Vec2 size, int texture_id = 0);

	THORUI_API void Draw();
	THORUI_API void OnItemEvent(Item_Event event) {}

	//Setters
	THORUI_API void SetColor(Color color);
	THORUI_API void SetTexture(uint texture_id);

	//Getters
	THORUI_API Color GetColor() const;
	THORUI_API uint GetTexID() const;

	THORUI_API void Save(Config& config);
	THORUI_API void Load(Config& config);

	THORUI_API virtual Appearance_Set SaveCurrentApState();
	THORUI_API virtual void UpdateApSetTransition(float dt);
public:

private:
	uint texture_id = 0;
	Color color; 
};

#endif // !__UI_IMAGE_H__

