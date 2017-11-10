#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "Vec2.h"
#include <string>
#include <vector>

struct SDL_Window;
class UI_Item;
typedef union SDL_Event;
struct ImFont;
class UI_Image;
class UI_Button;
class UI_Text;
class Dock;

namespace ThorUI
{
	struct Texture;
}

class UI_Editor
{
public:
	UI_Editor();
	~UI_Editor();

	bool Init(SDL_Window* window);
	void Draw();
	void ProcessEvent(SDL_Event* event);
	bool CleanUp();

private:
	void DrawHierarchy();
	void DrawHierarchyNode(UI_Item* item);
	void DrawHierarchyChilds(UI_Item* item);

	void DrawInspector();
	void DrawItemData(UI_Item* item);
	void DrawInspectorImage(UI_Image* img);
	void DrawInspectorText(UI_Text* text);
	void DrawInspectorButton(UI_Button* button);

	void DisplayTexture(ThorUI::Texture* tex);

	std::string OpenFileDialog() const;

public:

private:
	SDL_Window* window = nullptr;
	Vec2 window_size;
	Vec2 default_img_size_ratio = Vec2(0.3, 0.3f);
	UI_Item* selected = nullptr;
	ImFont* bold_font = nullptr;

	bool canvas_win = false;

	std::vector<Dock*> docks;
};

#endif //__EDITOR_H__
