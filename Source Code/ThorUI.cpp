#include "ThorUI.h"
#include "SDL2-2.0.6\include\SDL.h"

void ThorUI::Init()
{
	keys = new Key_State[SDL_NUM_SCANCODES];
}

void ThorUI::UpdateKeyboardState()
{
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
}

void ThorUI::GetEvent(SDL_Event event)
{

}
