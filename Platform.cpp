#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef WIN32
	#include <windows.h>
	#include <GL\gl.h>
	#include <GL\glu.h>
	#include "SDL2\SDL.h"
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <SDL2/SDL.h>
#else //UNIX
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <SDL2/SDL.h>
#endif

#include "Lib.h"
#include "Game.h"
#include "Platform.h"

bool keys[256];
int keypressed[256];

bool fullscreen=true;

unsigned long int frame=0;//??
int fps = 0;//Frames per second
int dticks=0;//
int frameskip=0;

int ticks, ptime, tfps;

//Screen width and height
int rxmax=0, rymax=0;

bool get_key(int id) /* export */
{
	assert((id >= 0) && (id < 256));
	return keys[id];
}

int get_keypressed(int id) /* export */
{
	assert((id >= 0) && (id < 256));
	return keypressed[id];
}

void ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	height = height?height:1;							// Prevent A Divide By Zero By
	
	glViewport(0, 0, width, height);					// Reset The Current Viewport
	
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	
	glOrtho(0.0f, width,height, 0.0f, -1.0f, 1.0f);		// Create Ortho 640x400 View (0,0 At Top Left)
	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void InitGL()											// All Setup For OpenGL Goes Here
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void DrawGLScene()										// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
}

SDL_Window* window;
SDL_GLContext context;

void PLT_CreateWindow()
{
	int x = SDL_WINDOWPOS_UNDEFINED, y = SDL_WINDOWPOS_UNDEFINED;
	Uint32 flags = SDL_WINDOW_OPENGL | (fullscreen?SDL_WINDOW_FULLSCREEN:0);
	window = SDL_CreateWindow("OpenGL window", x, y, rxmax, rymax, flags);
	if(!window)
	{
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		exit(1);
	}
	
	if(fullscreen)
	{
		SDL_ShowCursor(SDL_DISABLE);
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
	
	context = SDL_GL_CreateContext(window);
	if(!context)
	{
		fprintf(stderr, "Could not create OpenGL context: %s\n", SDL_GetError());
		exit(1);
	}
	
	ReSizeGLScene(rxmax, rymax);
	InitGL();
}

void PLT_DestroyWindow()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

void reload() /* export */
{
	PLT_DestroyWindow();
	PLT_CreateWindow();
}

void SwapBuffers()
{
	SDL_GL_SwapWindow(window);
}

void Quit(int exitcode)
{
	/* destroy() from Lib.cpp */
	destroy();
	PLT_DestroyWindow();
	exit(exitcode);
}

#define CLKPS 1000

void Sleep(int ms)
{
	SDL_Delay(ms);
}

void FPSCounter() // Black magic
{
	frame++;
	//printf("FPSCounter: frame=%i tfps=%i fps=%i dticks=%i ticks=%i frameskip=%i\n", frame, tfps, fps, dticks, ticks, frameskip);
	tfps++;
	if (ptime+1 == SDL_GetTicks()/CLKPS)
	{
		ptime++;
		fps = tfps;
		tfps = 0;
	}
	
	dticks=SDL_GetTicks()-ticks;
	if (dticks == 0) dticks=12;
	if (frameskip > 0)
	{	
		if ((CLKPS/(60))*frameskip > dticks)
		{
			frameskip--;
		}
	}
rtr:
	if ((CLKPS/(60))*(frameskip+1) > dticks)
	{
		if (((CLKPS/60) - dticks) > 0) Sleep(CLKPS/(60) - dticks);
	}
	else
	{
		if (frameskip <= 5) 
		{
			frameskip++;
			goto rtr;
		}
	}
	ticks=SDL_GetTicks();
}

int main(int argc, char** argv)
{
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		fprintf(stderr, "Unable to initialize SDL:  %s\n", SDL_GetError());
        return 1;
	}
	atexit(SDL_Quit);
	
	/* set_read() from Game.cpp */
	if(!set_read())
	{
		fprintf(stderr, "Failed to load settings\n");
        return 1;							
	}
	
	PLT_CreateWindow();
	
	/* init() from Game.cpp */
	if (init() != 0)
	{
		Quit(1);
	}
	
	ticks=SDL_GetTicks();
	while(1)
	{
		FPSCounter();

		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					printf("KEY_DOWN: %i\n", event.key.keysym.scancode);
					if(event.key.keysym.scancode < 256)
					{
						keys[event.key.keysym.scancode] = true;
					}
					break;
				case SDL_KEYUP:
					printf("KEY_UP: %i\n", event.key.keysym.scancode);
					if(event.key.keysym.scancode < 256)
					{
						keypressed[event.key.keysym.scancode] = 0;
						keys[event.key.keysym.scancode] = false;
					}
					break;
				case SDL_QUIT:
					Quit(0);
					break;
			}
		}
		
		/* Game.cpp */
		OP();		
		KOP();
		
		for(int i=0; i<256; i++, keys[i]?keypressed[i]++:keypressed[i]=0);
	}
	
	return 0;
}
