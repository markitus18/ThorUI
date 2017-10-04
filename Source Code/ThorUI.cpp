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
	Key_State* mouse_buttons = nullptr;
	std::vector<UI_Item*> items;
	Vec2 mouse_pos = Vec2(-1000, -1000);
	Vec2 screen_size;
	bool mouse_out = false;
	bool breakpoint = false;

	void Init(SDL_Window* window)
	{
		gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);

		keyboard = new Key_State[SDL_NUM_SCANCODES];
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
			keyboard[i] = KEY_IDLE;
		mouse_buttons = new Key_State[3];
		for (int i = 0; i < 3; ++i)
			mouse_buttons[i] = KEY_IDLE;
		
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		screen_size.Set(w, h);
	}

	void StartFrame()
	{
		UpdateKeyboardState();
		UpdateMouseState();
		UpdateItems();
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
		if (mouse_out == false)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			mouse_pos.Set(x/screen_size.x, (1 - y/screen_size.y)); //Setting mouse to [0, 1]. Y must be inverted (SDL 0y is top)
		}

		for (int i = 0; i < 3; ++i)
		{
			if (mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_REPEAT;
			if (mouse_buttons[i] == KEY_UP)
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	Key_State GetMouseState(int id)
	{
		return mouse_buttons[id - 1];
	}

	bool IsMouseDown(int id)
	{
		return (mouse_buttons[id - 1] == KEY_DOWN || mouse_buttons[id - 1] == KEY_REPEAT);
	}

	bool IsMouseIdle(int id)
	{
		return (mouse_buttons[id - 1] == KEY_UP || mouse_buttons[id - 1] == KEY_IDLE);
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
		switch (event.type)
		{
			case(SDL_MOUSEBUTTONUP):
			{
				mouse_buttons[event.button.button - 1] = KEY_UP;
				break;
			}
			case(SDL_MOUSEBUTTONDOWN):
			{
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
				break;
			}
			case(SDL_WINDOWEVENT):
			{
				switch (event.window.event)
				{
					case(SDL_WINDOWEVENT_LEAVE):
					{
						LOG("Mouse leaving window");
						mouse_pos.Set(-1000, -1000);
						mouse_out = true;
						break;
					}
					case(SDL_WINDOWEVENT_ENTER):
					{
						LOG("Mouse enter");
						mouse_out = false;
						break;
					}
					case(SDL_WINDOWEVENT_FOCUS_GAINED):
					{
						//If we gain focus and mouse is inside the window, it was just clicked.
						//SDL doesnt detect that click, it is faked here
						//TODO: detect which button was pressed to gain focus
						if (mouse_out == false)
						{
							SDL_Event ev;
							ev.type = SDL_MOUSEBUTTONDOWN;
							ev.button.button = SDL_BUTTON_LEFT;
							SDL_PushEvent(&ev);
						}
						LOG("Focus gain");
					}
				}
			}
		}
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
		return (mouse_pos.x > pos.x && mouse_pos.x < pos.x + size.x &&
			mouse_pos.y > pos.y && mouse_pos.y < pos.y + size.y);
	}

	void AddItem(UI_Item* item)
	{
		items.push_back(item);
	}

	void UpdateItems()
	{
		std::vector<UI_Item*>::iterator it;
		for (it = items.begin(); it != items.end(); ++it)
		{
			Item_Event last_event = (*it)->GetLastEvent();
			if (IsMouseHovering((*it)->GetPos(), (*it)->GetSize())) //Mouse is hovering item
			{
				if (last_event == Mouse_Exit)
					(*it)->OnItemEvent(Mouse_Enter);
				if (last_event == Mouse_Down && IsMouseIdle(SDL_BUTTON_LEFT))
					(*it)->OnItemEvent(Mouse_Up);
				if ((last_event == Mouse_Enter || last_event == Mouse_Up) && IsMouseDown(SDL_BUTTON_LEFT))
					(*it)->OnItemEvent(Mouse_Down);
			}
			else //Mouse is not hovering item
			{
				if (last_event != Mouse_Exit)
					(*it)->OnItemEvent(Mouse_Exit);
			}
		}
		}
	}

