#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

#include "UI_Item.h"
#include "Color.h" //TODO: fwd declaration and Color* ?

class UI_Image : public UI_Item
{
public:
	UI_Image() { name = "Image"; };
	UI_Image(Vec2 pos, Vec2 size, int texture_id = 0);

	void Draw();
	
	//Setters
	void SetColor(Color color);
	void SetTexture(uint texture_id);

	uint GetTexID() const;

private:
	uint texture_id = 0;

public:
	Color color; 
};

#endif // !__UI_IMAGE_H__

