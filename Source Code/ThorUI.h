#ifndef __THORUI_H__
#define __THORUI_H__

#include <vector>
#include <map>

#include "Vec2.h"
#include <string>

typedef union SDL_Event;
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
		Font(_TTF_Font* font, const char* path, uint size): size(size), font(font), path(path){}
		_TTF_Font* font = nullptr;
		uint size = 0;
		uint instances = 0;
		std::string path;
	};

	extern Key_State* keyboard;
	extern Key_State* mouse_buttons;
	extern bool* mouse_button_event;
	extern Vec2 mouse_pos;
	extern Vec2 last_mouse_click;
	extern bool mouse_out;
	extern bool mouse_dragging;

	extern Vec2 screen_size;

	extern UI_Item* window_item;
	extern std::vector<UI_Item*> items;
	extern std::vector<UI_Item*> to_remove_items;
	extern std::map<uint, Texture> textures; //TODO: sort textures by ID or by path?
	extern std::vector<Font> fonts;

	void Init(SDL_Window* window);
	void StartFrame();
	void Draw();
	void CleanUp();

	//Keyboard event handling -------------------------
	void UpdateKeyboardState();
	void DebugKeyboardState(int key, Key_State state);
	Key_State GetKeyState(int key);
	void UpdateMouseState();
	//-------------------------------------------------

	//Mouse event handling ----------------------------
	Key_State GetMouseState(int id);
	bool IsMouseDown(int id);
	bool IsMouseIdle(int id);

	bool IsMouseHovering(Vec2 pos, Vec2 size);
	bool IsMouseHoveringItem(UI_Item* item);
	bool IsPointOnItem(UI_Item* item, Vec2 p);
	//-------------------------------------------------

	//Texture and text handling -----------------------
	uint LoadTexture(const char* path);

		//* Find a loaded texture index by it's path
		//* Returns 0 if not successful
	uint FindTexture(const char* path);

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

		//* returns a font pointer based on path and size
		//* returns nullptr on failure
	uint FindFont(const char* path, uint size);

		//* Adds a font instance
	void OnSetFont(uint font_id);

		//* Removes a font instance
	void OnLeaveFont(uint font_id);

		//* returns loaded OpenGL texture
		//* color will be added as a tint on the texture when drawing
		//* returns 0 on failure
	uint GenTextTexture(const char* text, uint font, Vec2& texture_size, Vec2 boundaries_size);

		//* returns an empty generated OpenGL texture buffer
		//* newly generated texture is binded, needs to be unbinded later
	uint GenTexture();

		//* Generates an OpenGL texture buffer from an SDL_Surface
		//* The surface parameter will be modified
		//* TODO: revert surface modification
	uint GenTextureFromSurf(SDL_Surface* surf);
	//-------------------------------------------------

	void GetEvent(SDL_Event* event);
	
	//Draw Utilities ----------------------------------
	void DrawQuad(Vec2 pos, Vec2 size, Color color, bool filled = true, float lineWidth = 1.0f);
	//void DrawQuad(Vec2* corners, Color color, bool filled = true, float lineWidth = 1.0f);
	void DrawImage(Vec2 pos, Vec2 size, int texture_id, Color color);
	//void DrawImage(Vec2* corners, int texture_id, Color  color);
	//-------------------------------------------------

	//Item management ---------------------------------
	void AddItem(UI_Item* item);
	std::string GenUniqueName(UI_Item* parent, const char* name);
	void UpdateItems();
	void DeleteItem(UI_Item* item);
	void RemoveToDeleteItems();
	//-------------------------------------------------

	//Secen management --------------------------------
	void SaveScene(const char* path);
	void LoadScene(const char* path);
	void ClearScene();
	void SetCanvasSize(Vec2 size);
	//-------------------------------------------------

};

#endif