#ifndef __THORUI_H__
#define __THORUI_H__

enum SDL_Event;


namespace ThorUI
{
	enum Key_State
	{
		KEY_IDLE,
		KEY_DOWN,
		KEY_REPEAT,
		KEY_UP
	};

	Key_State* keys;

	void Init();
	void UpdateKeyboardState();
	void GetEvent(SDL_Event event);
	
};

#endif