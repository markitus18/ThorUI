#ifndef __THORUI_H__
#define __THORUI_H__

typedef union SDL_Event;
struct Vec2;
struct Color;

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


	void Init();

	//Keyboard event handling -------------------------
	void UpdateKeyboardState();
	void DebugKeyboardState(int key, Key_State state);
	Key_State GetKeyState(int key);
	//-------------------------------------------------

	//Texture handling --------------------------------
	int LoadTexture(char* path);
	void FreeTexture(int texture_id);
	//-------------------------------------------------

	void GetEvent(SDL_Event event);
	
	//Draw Utilities
	void DrawQuad(Vec2 pos, Vec2 size, Color color);
	void DrawImage(Vec2 pos, Vec2 size, int texture_id);

};

#endif