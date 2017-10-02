#include "ThorUI.h"
#include "SDL2-2.0.6\include\SDL.h"
#include "Log.h"
#include <string>


namespace ThorUI
{
	Key_State* keyboard = nullptr;

	void Init()
	{
		keyboard = new Key_State[SDL_NUM_SCANCODES];
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
			keyboard[i] = KEY_IDLE;
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
				DebugKeyboardState(i, keyboard[i]);
			}
			else
			{
				if (keyboard[i] != KEY_IDLE)
				{
					keyboard[i] = (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
					DebugKeyboardState(i, keyboard[i]);
				}
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

	void GetEvent(SDL_Event event)
	{

	}
}

