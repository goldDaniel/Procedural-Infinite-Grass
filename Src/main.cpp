#include <cstdio>
#include <cstdlib>
 
#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
 
#include "camera.h"
#include "renderer.h"
#include "glad/glad.h"

static const int SCREEN_FULLSCREEN = 0;
static const int SCREEN_WIDTH  = 1280;
static const int SCREEN_HEIGHT = 720;
static SDL_Window *window = NULL;
static SDL_GLContext maincontext;
 
Camera* cam; 

void initScreen(const char * caption) 
{
	// Initialize SDL 
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL\n");
		exit(2);
	}
	atexit (SDL_Quit);
	SDL_GL_LoadLibrary(NULL); 

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create the window
	if(SCREEN_FULLSCREEN) 
	{
		window = SDL_CreateWindow(
			caption, 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
		);
	} 
	else 
	{
		window = SDL_CreateWindow(
			caption, 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL
		);
	}
	if(window == NULL)
	{
		printf("Couldn't set video mode\n");
		exit(2);
	} 

	maincontext = SDL_GL_CreateContext(window);
	if(maincontext == NULL) 
	{
		printf("Failed to create OpenGL context\n");
		exit(2);
	}
	

	// Check OpenGL properties
	printf("OpenGL loaded\n");
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	// Use v-sync
	SDL_GL_SetSwapInterval(1);

	// Disable depth test and face culling.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	int w,h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
}


int main() 
{
	initScreen("OpenGL 3.3");
	
	cam = new Camera();
	Renderer::Init();
	
	SDL_Event event;
	
	bool quit = false;


	float prevTime = SDL_GetTicks() / 1000.f;

	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

	while (!quit) 
	{
		float currTime = SDL_GetTicks() / 1000.f;
		float dt = currTime - prevTime;
		prevTime = currTime;
		
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT) 
			{
				quit = true;
			}

			if(event.type == SDL_MOUSEMOTION)
			{
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					cam->processMouseMovement(event.motion.xrel, event.motion.yrel);
				}	
			}

			if(event.type == SDL_KEYDOWN)
			{
				switch( event.key.keysym.sym )
				{
                    case SDLK_a:
						left = true;
                        break;
                    case SDLK_d:
						right = true;
                        break;
                    case SDLK_w:
						up = true;
                        break;
                    case SDLK_s:
						down = true;
                        break;
                    default:
                        break;
                }		
			}
			if(event.type == SDL_KEYUP)
			{
				switch( event.key.keysym.sym )
				{
                    case SDLK_a:
						left = false;
                        break;
                    case SDLK_d:
						right = false;
                        break;
                    case SDLK_w:
						up = false;
                        break;
                    case SDLK_s:
						down = false;
                        break;
                    default:
                        break;
                }		
			}
		}

		if(left)
		{	
			cam->processKeyboard(Camera_Movement::LEFT, dt);
		}	
		if(right)
		{
			cam->processKeyboard(Camera_Movement::RIGHT, dt);
		}
		if(up)
		{
			cam->processKeyboard(Camera_Movement::FORWARD, dt);
		}	
		if(down)
		{
			cam->processKeyboard(Camera_Movement::BACKWARD, dt);
		}
		

		Renderer::Draw(cam->getViewMatrix());

		SDL_GL_SwapWindow(window);
	}
}


