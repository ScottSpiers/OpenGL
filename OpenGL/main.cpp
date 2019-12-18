

#include <iostream>
#include <string>
#include <SDL.h>
#include <glew.h>

void InitSystems();
void ProcessInput(); 
void ReportError(std::string);
void Render(SDL_Window*);

bool shouldExit = false;

int main(int args, char** argv)
{

	InitSystems();

	SDL_Window* m_window = 0;
	int screenWidth = 1280;
	int screenHeight = 720;

	m_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

	if (!m_window)
		ReportError("Window could not be created");

	SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
	if (!glContext)
	{
		ReportError("SDL_GL context could not be created");
	}

	GLenum result = glewInit();
	if (result != GLEW_OK)
		ReportError("Could not initialise GLEW");

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	while (!shouldExit)
	{
		ProcessInput();
		Render(m_window);
	}

	return 0;
}

void InitSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

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

void ReportError(std::string str_error)
{
	std::cout << str_error << std::endl;
	std::cout << "Enter any key to quit...";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
}

void Render(SDL_Window* win)
{	
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_COLOR_ARRAY);
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(-0.25f, 0.5f);
	glVertex2f(-0.25f, -0.25f);
	glVertex2f(0.25f, 0.5f);

	//inefficient (could resue existing vertices)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.25f, 0.5f);
	glVertex2f(-0.25f, -0.25f);
	glVertex2f(0.25f, -0.25f);


	glEnd();

	SDL_GL_SwapWindow(win);
}