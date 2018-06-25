#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <string>
#include <vector>

#include "Vec2.h"

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

	Vec2 GetClosestGridPoint(int x, int y);
	void OnButtonClickedTest();
	void OnButtonHoverTest(int val);

private:
	void HandleInput();
	void DrawMainMenuBar();
	void DrawIconBar();
	void DrawGrid();

	void DrawCanvasWindow();
	void DrawGridWindow();
	void DrawSnapWindow();
	void DrawSaveWindow();

	void DeleteSelected();

public:
	Scene* scene = nullptr;
	Vec2 window_size;
	bool dev_tools = false;

	//Snap settings
	int angle_interval = 15;
	float scale_interval = 0.5;

	//ImGui data
	bool capture_mouse = false;
	bool capture_keyboard = false;

	Hierarchy* hierarchy = nullptr;
private:
	SDL_Window* window = nullptr;
	Vec2 default_img_size_ratio = Vec2(0.3, 0.3f);

	ImFont* bold_font = nullptr;

	//Windows display
	bool canvas_win = false;
	bool grid_win = false;
	bool snap_win = false;
	bool save_win = false;

	//Grid edition variables
	bool grid = true;
	Vec2 grid_separation = Vec2(60, 60);
	Vec2 grid_div;
	bool block_grid = true;

	std::vector<Dock*> docks;

	Inspector* inspector = nullptr;
	Resources* resources = nullptr;
	Dock* dock_focus = nullptr;

	bool run_simulation = false;
	//delete marc;
};

extern UI_Editor* editor;

#endif //__EDITOR_H__
