#ifndef __THORUI_H__
#define __THORUI_H__

typedef union SDL_Event;


namespace ThorUI
{
	enum Key_State
	{
		KEY_IDLE,
		KEY_DOWN,
		KEY_REPEAT,
		KEY_UP
	};
	extern Key_State* keyboard;

	void Init();
	void UpdateKeyboardState();
	void DebugKeyboardState(int key, Key_State state);

	void GetEvent(SDL_Event event);
	
};

#endif