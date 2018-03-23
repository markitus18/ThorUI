#ifndef __THOR_UI_H__
#define __THOR_UI_H__

#ifdef THORUI_EXPORTS
#define THORUI_API __declspec(dllexport) 
#else
#define THORUI_API __declspec(dllimport) 
#endif

#include <vector>
#include <map>

#include "Vec2.h"
#include <string>

union SDL_Event;
struct _TTF_Font;
struct SDL_Window;
struct SDL_Surface;

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

	struct Font
	{
		Font(_TTF_Font* font, const char* path, uint size) : size(size), font(font), path(path) {}
		_TTF_Font* font = nullptr;
		uint size = 0;
		uint instances = 0;
		std::string path;
	};

	extern THORUI_API Key_State* keyboard;
	extern THORUI_API Key_State* mouse_buttons;
	extern THORUI_API bool* mouse_button_event;
	extern THORUI_API Vec2 mouse_pos;
	extern THORUI_API Vec2 last_mouse_click;
	extern THORUI_API bool mouse_out;
	extern THORUI_API bool mouse_dragging;

	extern THORUI_API Vec2 screen_size;

	extern THORUI_API UI_Item* window_item;
	extern THORUI_API std::vector<UI_Item*> items;
	extern THORUI_API std::vector<UI_Item*> to_remove_items;
	extern THORUI_API std::map<uint, Texture> textures; //TODO: sort textures by ID or by path?
	extern THORUI_API std::vector<Font> fonts;

	THORUI_API void Init(SDL_Window* window);
	THORUI_API void StartFrame();
	THORUI_API void PreStart();
	THORUI_API void Update();
	THORUI_API void Draw();
	THORUI_API void CleanUp();

	//Keyboard event handling -------------------------
	THORUI_API void UpdateKeyboardState();
	THORUI_API void DebugKeyboardState(int key, Key_State state);
	THORUI_API Key_State GetKeyState(int key);
	THORUI_API void UpdateMouseState();
	//-------------------------------------------------

	//Mouse event handling ----------------------------
	THORUI_API Key_State GetMouseState(int id);
	THORUI_API bool IsMouseDown(int id);
	THORUI_API bool IsMouseIdle(int id);

	THORUI_API bool IsMouseHovering(Vec2 pos, Vec2 size);
	THORUI_API bool IsMouseHoveringItem(UI_Item* item);
	THORUI_API bool IsPointOnItem(UI_Item* item, Vec2 p);
	//-------------------------------------------------

	//Texture and text handling -----------------------
	THORUI_API uint LoadTexture(const char* path);

	//* Find a loaded texture index by it's path
	//* Returns 0 if not successful
	THORUI_API uint FindTexture(const char* path);

	//* Get a loaded texture by it's id
	//* Returns nullptr if not successful
	THORUI_API Texture* GetTexture(uint texture_id);

	//* Release a texture from memory
	THORUI_API void FreeTexture(uint texture_id);

	//* Adds a texture instance
	THORUI_API void OnSetTexture(uint texture_id);

	//* Removes a texture instance
	THORUI_API void OnLeaveTexture(uint texture_id);

	//* returns a font index, used in LoadTextTexture
	//* returns 0 on failure
	THORUI_API uint LoadFont(const char* path, uint size);

	//* returns a font pointer based on path and size
	//* returns nullptr on failure
	THORUI_API uint FindFont(const char* path, uint size);

	//* Adds a font instance
	THORUI_API void OnSetFont(uint font_id);

	//* Removes a font instance
	THORUI_API void OnLeaveFont(uint font_id);

	//* returns loaded OpenGL texture
	//* color will be added as a tint on the texture when drawing
	//* returns 0 on failure
	THORUI_API uint GenTextTexture(const char* text, uint font, Vec2& texture_size, Vec2 boundaries_size);

	//* returns an empty generated OpenGL texture buffer
	//* newly generated texture is binded, needs to be unbinded later
	THORUI_API uint GenTexture();

	//* Generates an OpenGL texture buffer from an SDL_Surface
	//* The surface parameter will be modified
	//* TODO: revert surface modification
	THORUI_API uint GenTextureFromSurf(SDL_Surface* surf);
	//-------------------------------------------------

	THORUI_API void GetEvent(SDL_Event* event);

	//Draw Utilities ----------------------------------
	THORUI_API void DrawQuad(Vec2 pos, Vec2 size, Color color, bool filled = true, float lineWidth = 1.0f);
	//void DrawQuad(Vec2* corners, Color color, bool filled = true, float lineWidth = 1.0f);
	THORUI_API void DrawImage(Vec2 pos, Vec2 size, int texture_id, Color color);
	//void DrawImage(Vec2* corners, int texture_id, Color  color);
	//-------------------------------------------------

	//Item management ---------------------------------
	THORUI_API void AddItem(UI_Item* item);
	THORUI_API std::string GenUniqueName(UI_Item* parent, const char* name);
	THORUI_API UI_Item* GetItem(uint item_id);

	//* "Tags" the item (and it's children) to be safely deleted after
	//* It removes the hole hierarchy pointers, do not try to access them after calling this function
	THORUI_API void DeleteItem(UI_Item* item);
	THORUI_API void RemoveToDeleteItems();
	//-------------------------------------------------

	//Secen management --------------------------------
	THORUI_API void SaveScene(const char* path);
	THORUI_API void LoadScene(const char* path);
	THORUI_API void ClearScene();
	THORUI_API void SetCanvasSize(Vec2 size);
	//-------------------------------------------------

};

#endif //__THOR_UI_H__