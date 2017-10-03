#ifndef __THORUI_H__
#define __THORUI_H__

#include <vector>

typedef union SDL_Event;
struct Vec2;
struct Color;
class UI_Item;

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
	extern std::vector<UI_Item*> items;
	extern Vec2 mouse_pos;

	void Init();
	void StartFrame();
	void Draw();

	//Input event handling -------------------------
	void UpdateKeyboardState();
	void DebugKeyboardState(int key, Key_State state);
	Key_State GetKeyState(int key);
	void UpdateMouseState();
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

	//Event Utilities ---------------------------------
	bool IsMouseHovering(Vec2 pos, Vec2 size);
	//-------------------------------------------------

	//Item management ---------------------------------
	void AddItem(UI_Item* item);
	//-------------------------------------------------
};

#endif