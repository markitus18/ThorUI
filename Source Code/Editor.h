#ifndef __EDITOR_H__
#define __EDITOR_H__

struct SDL_Window;

class UI_Editor
{
public:
	UI_Editor();
	~UI_Editor();

	bool Init(SDL_Window* window);
	void Draw();
	bool CleanUp();

private:

public:

private:
	SDL_Window* window;
};

#endif //__EDITOR_H__
