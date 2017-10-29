#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Vec2.h"

struct SDL_Window;
class UI_Item;

class UI_Editor
{
public:
	UI_Editor();
	~UI_Editor();

	bool Init(SDL_Window* window);
	void Draw();
	bool CleanUp();

private:
	void DrawHierarchy();
	void DrawHierarchyNode(UI_Item* item);

	void DrawInspector();
	void DrawItemData(UI_Item* item);
	
public:
	bool created = false;
private:
	SDL_Window* window;
	Vec2 window_size;
	Vec2 default_img_size_ratio = Vec2(0.3, 0.3f);
	UI_Item* selected;
};

#endif //__EDITOR_H__
