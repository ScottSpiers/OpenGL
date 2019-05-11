
#include <SDL.h>
#include <glew.h>

#include <iostream>

void ProcessInput(); 

bool shouldExit = false;

int main(int args, char** argv)
{

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* m_window;
	int screenWidth = 1920;
	int screenHeight = 1080;

	SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

	while (!shouldExit)
	{
		ProcessInput();
	}

	return 0;
}

void ProcessInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT: shouldExit = true;
		default:
			break;
		}
	}

}