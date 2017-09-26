#include <iostream>
#include "SDL2-2.0.6\include\SDL.h"
#include "glew-2.1.0\include\GL\glew.h"

#pragma comment( lib, "SDL2-2.0.6/libx86/SDL2.lib" )
#pragma comment( lib, "SDL2-2.0.6/libx86/SDL2main.lib" )

#pragma comment( lib, "glew-2.1.0/libx86/glew32.lib" )

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gHelloWorld = nullptr;

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
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 500, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}


//	SDL_GL_CreateContext()
	GLenum error = glewInit();

	if (error != GL_NO_ERROR)
	{
		//printf("Error initializing glew library! %s", glGetError());
		success = false;
	}

	return success;
}

bool load()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	gHelloWorld = SDL_LoadBMP("photo.bmp");
	if (gHelloWorld == NULL)
	{
		const char* error = SDL_GetError();
		printf("Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

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
		//Load media
		if (!load())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Apply the image
			SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
			SDL_UpdateWindowSurface(gWindow);
			SDL_Delay(5000);


		}
	}

	close();
	return 0;

}