#include <iostream>
#include "SDL2-2.0.6\include\SDL.h"
#include "glew-2.1.0\include\GL\glew.h"

#pragma comment( lib, "SDL2-2.0.6/libx86/SDL2.lib" )
#pragma comment( lib, "SDL2-2.0.6/libx86/SDL2main.lib" )

#pragma comment( lib, "glew-2.1.0/libx86/glew32.lib" )
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

#include "Vec2.h"
#include "Color.h"
#include "ThorUI.h"
#include "Editor.h"
#include "Log.h"

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* imageSurf = nullptr;
SDL_GLContext gContext;

UI_Image* image;
UI_Button* button;
UI_Text* text;

//UI_Editor* editor;

uint font_1;
uint font_2;

UI_Editor* editor = nullptr;

bool initGL()
{
	editor = new UI_Editor();

	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("Error initializing Glew! %s\n", glewGetErrorString(GlewInitResult));
		success = false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0)
		LOG("Unable to set VSYNC: %s", SDL_GetError());
	return success;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("Thor UI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |SDL_WINDOW_RESIZABLE);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gContext = SDL_GL_CreateContext(gWindow);
			initGL();
			//gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(imageSurf);
	imageSurf = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char** args)
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		static SDL_Event ev;

		bool quit = false;
		if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
		{
			printf("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		}

		SDL_StartTextInput();
		//ThorUI::Init(gWindow);
		editor->Init(gWindow);

		while (quit == false)
		{
			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			while (SDL_PollEvent(&ev) != 0)
			{
				ThorUI::GetEvent(&ev);
				editor->ProcessEvent(&ev);
				if (ev.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			//ThorUI::StartFrame();
			//ThorUI::Draw();

			editor->Draw();

			SDL_GL_SwapWindow(gWindow);
			ThorUI::UpdateKeyboardState();
		}
	}
	//SDL_StopTextInput();
	close();
	return 0;

}