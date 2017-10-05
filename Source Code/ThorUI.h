#ifndef __THORUI_H__
#define __THORUI_H__

#include <vector>

typedef union SDL_Event;
struct Vec2;
struct Color;
class UI_Item;
struct SDL_Window;

enum Key_State
{
	KEY_IDLE,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

namespace ThorUI
{
	extern Key_State* keyboard;
	extern Key_State* mouse_buttons;
	extern bool* mouse_button_event;

	extern Vec2 screen_size;
	extern bool breakpoint;
	extern std::vector<UI_Item*> items;

	extern Vec2 mouse_pos;
	extern bool mouse_out;

	void Init(SDL_Window* window);
	void StartFrame();
	void Draw();

	//Keyboard event handling -------------------------
	void UpdateKeyboardState();
	void DebugKeyboardState(int key, Key_State state);
	Key_State GetKeyState(int key);
	void UpdateMouseState();
	//-------------------------------------------------

	//Mouse event handling ----------------------------
	bool IsMouseHovering(Vec2 pos, Vec2 size);
	Key_State GetMouseState(int id);
	bool IsMouseDown(int id);
	bool IsMouseIdle(int id);
	//-------------------------------------------------

	//Texture handling --------------------------------
	int LoadTexture(char* path);
	void FreeTexture(int texture_id);
	//-------------------------------------------------

	void GetEvent(SDL_Event event);
	
	//Draw Utilities ----------------------------------
	void DrawQuad(Vec2 pos, Vec2 size, Color color);
	void DrawImage(Vec2 pos, Vec2 size, int texture_id, Color color);
	//-------------------------------------------------

	//Item management ---------------------------------
	void AddItem(UI_Item* item);
	void UpdateItems();
	//-------------------------------------------------
};

#endif