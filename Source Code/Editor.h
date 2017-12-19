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

namespace ThorUI{struct Texture;}
typedef unsigned int uint;
typedef unsigned int ImU32;

class Scene;
class Inspector;
class Hierarchy;
class Resources;
struct Rect;
struct ImVec2;
struct Vec2;
struct Circle;

class UI_Editor
{
public:
	UI_Editor();
	~UI_Editor();

	bool Init(SDL_Window* window);
	void Draw();
	void ProcessEvent(SDL_Event* event);
	bool CleanUp();

	void DisplayTexture(ThorUI::Texture* tex);
	void DrawRect(Rect rect, ImU32 color, bool filled = true, float thickness = 1.0f);
	void DrawTriangle(Vec2 a, Vec2 b, Vec2 c, ImU32 color);
	void DrawCircle(Circle circle, ImU32 color);

	void SetDockFocus(Dock* dock);

	ImVec2 ToImVec2(Vec2 point);
	Vec2 ToVec2(ImVec2 point);

private:
	void DrawMainMenuBar();
	void DrawIconBar();

public:
	UI_Item* selected = nullptr;
	Scene* scene = nullptr;
	Vec2 window_size;

private:

	SDL_Window* window = nullptr;
	Vec2 default_img_size_ratio = Vec2(0.3, 0.3f);

	ImFont* bold_font = nullptr;

	bool canvas_win = false;

	std::vector<Dock*> docks;

	Inspector* inspector = nullptr;
	Hierarchy* hierarchy = nullptr;
	Resources* resources = nullptr;

	Dock* dock_focus = nullptr;
		//delete marc;
};

extern UI_Editor* editor;

#endif //__EDITOR_H__
