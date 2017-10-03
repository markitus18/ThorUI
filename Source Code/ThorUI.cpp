#include "ThorUI.h"
#include "SDL2-2.0.6\include\SDL.h"
#include "Log.h"
#include "Vec2.h"
#include "Color.h"
#include "glew-2.1.0\include\GL\glew.h"
#include <string>
#include "UI_Item.h"

namespace ThorUI
{
	Key_State* keyboard = nullptr;
	std::vector<UI_Item*> items;
	void Init()
	{
		keyboard = new Key_State[SDL_NUM_SCANCODES];
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
			keyboard[i] = KEY_IDLE;
	}

	void StartFrame()
	{
		UpdateKeyboardState();
	}

	void Draw()
	{
		std::vector<UI_Item*>::iterator it;
		for (it = items.begin(); it != items.end(); ++it)
		{
			(*it)->Draw();
		}
	}

	void CleanUp()
	{
		delete[] keyboard;
		keyboard = nullptr;
	}

	void UpdateKeyboardState()
	{
		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		for (unsigned int i = 0; i < SDL_NUM_SCANCODES; ++i)
		{
			if (keys[i] == 1)
			{
				keyboard[i] = (keyboard[i] == KEY_DOWN || keyboard[i] == KEY_REPEAT) ? KEY_REPEAT : KEY_DOWN;
				//DebugKeyboardState(i, keyboard[i]);
			}
			else if(keyboard[i] != KEY_IDLE)
			{	
					keyboard[i] = (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
					//DebugKeyboardState(i, keyboard[i]);
			}
		}
	}
	
	//Just for debugging purposes: shows key state change output
	void DebugKeyboardState(int key, Key_State state)
	{
		std::string str = SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)key));

		char* key_str[4] = { "Idle", "Down", "Repeat", "Up" };
		str.append(" - Key ").append(key_str[state]);

		LOG(str.c_str());
	}

	Key_State GetKeyState(int key)
	{
		return keyboard[key];
	}

	void UpdateMouseState()
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		mouse_pos.Set(x, y);
	}

	int LoadTexture(char* path)
	{
		SDL_Surface* surf = SDL_LoadBMP(path);
		if (surf == nullptr)
		{
			LOG("Error loading texture: %s", SDL_GetError());
		}
		//TODO: Assuming it is GL_BGR, skipping checks by now

		//Generating texture buffer
		GLuint texture = 0;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Filling buffer data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surf->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SDL_FreeSurface(surf);

		return texture;
	}

	void ThorUI::FreeTexture(int texture_id)
	{
		glDeleteTextures(1, (GLuint*)&texture_id);
	}

	void GetEvent(SDL_Event event)
	{

	}

	void DrawQuad(Vec2 pos, Vec2 size, Color color)
	{
		glColor4fv(color.ptr());
		glBegin(GL_QUADS);
		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y + size.y);
		glVertex2f(pos.x, pos.y + size.y);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void DrawImage(Vec2 pos, Vec2 size, int texture_id, Color color)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);
		glColor4fv(color.ptr());
		glTexCoord2f(0.0, 1.0);	glVertex2f(pos.x, pos.y);
		glTexCoord2f(1.0, 1.0);	glVertex2f(pos.x + size.x, pos.y);
		glTexCoord2f(1.0, 0.0);	glVertex2f(pos.x + size.x, pos.y + size.y);
		glTexCoord2f(0.0, 0.0);	glVertex2f(pos.x, pos.y + size.y);
		glEnd();
		glColor4fv(Color::White().ptr());
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	bool IsMouseHovering(Vec2 pos, Vec2 size)
	{

	}

	void AddItem(UI_Item* item)
	{
		items.push_back(item);
	}
}

