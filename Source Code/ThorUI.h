#ifndef __THORUI_H__
#define __THORUI_H__

#include <vector>
#include <map>

#include "Vec2.h"
#include <string>

typedef union SDL_Event;
struct Vec2;
struct Color;
class UI_Item;
struct _TTF_Font;
struct SDL_Window;
struct SDL_Surface;

enum Key_State
{
	KEY_IDLE,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

typedef unsigned int uint;

namespace ThorUI
{
	struct Texture
	{
		uint id = 0;
		uint instances = 0;
		Vec2 original_size;
		std::string path;
	};

	extern Key_State* keyboard;
	extern Key_State* mouse_buttons;
	extern bool* mouse_button_event;

	extern Vec2 screen_size;
	extern bool breakpoint;

	extern std::vector<UI_Item*> items;
	extern std::vector<_TTF_Font*> fonts; //TODO: mesure the time loading a texture	
	extern std::map<uint, Texture> textures; //TODO: sort textures by ID or by path?

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

	//Texture and text handling -----------------------
	uint LoadTexture(const char* path);

		//* Find a loaded texture by it's path
		//* Returns nullptr if not successful
	Texture* FindTexture(const char* path);

		//* Get a loaded texture by it's id
		//* Returns nullptr if not successful
	Texture* GetTexture(uint texture_id);

		//* Release a texture from memory
	void FreeTexture(uint texture_id);

		//* Adds a texture instance
	void OnSetTexture(uint texture_id);

		//* Removes a texture instance
	void OnLeaveTexture(uint texture_id);

		//* returns a font index, used in LoadTextTexture
		//* returns 0 on failure
	uint LoadFont(const char* path, uint size);

		//* returns loaded OpenGL texture
		//* returns 0 on failure
		//* font given by return LoadFont(..)
	uint GenTextTexture(const char* text, uint font, Color color, Vec2& texture_size);

		//* returns an empty generated OpenGL texture buffer
		//* newly generated texture is binded, needs to be unbinded later
	uint GenTexture();

		//* Generates an OpenGL texture buffer from an SDL_Surface
		//* The surface parameter will be modified
		//* TODO: revert surface modification
	uint GenTextureFromSurf(SDL_Surface* surf);
	//-------------------------------------------------

	void GetEvent(SDL_Event event);
	
	//Draw Utilities ----------------------------------
	void DrawQuad(Vec2 pos, Vec2 size, Color color, bool filled = true, float lineWidth = 1.0f);
	void DrawImage(Vec2 pos, Vec2 size, int texture_id, Color color);
	//-------------------------------------------------

	//Item management ---------------------------------
	void AddItem(UI_Item* item);
	void UpdateItems();
	//-------------------------------------------------
};

#endif