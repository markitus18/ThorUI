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
typedef unsigned int uint;
class Scene;
class Inspector;
class Hierarchy;
class Resources;

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
	void DisplayTexture(ThorUI::Texture* tex);

	std::string OpenFileDialog() const;

public:
	UI_Item* selected = nullptr;

private:
	SDL_Window* window = nullptr;
	Vec2 window_size;
	Vec2 default_img_size_ratio = Vec2(0.3, 0.3f);

	ImFont* bold_font = nullptr;

	bool canvas_win = false;

	std::vector<Dock*> docks;

	uint tmp_buff = 0;

	Scene* s_dock = nullptr;
	Inspector* i_dock = nullptr;
	Hierarchy* h_dock = nullptr;
	Resources* r_dock = nullptr;
};

#endif //__EDITOR_H__
