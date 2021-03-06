#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

#include "glmath.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(json file);
	bool CleanUp();

	void SetTitle(const char* title);

	void GetWindowSize(int &width, int &height);
	vec2 GetWindowSize();

public:

	void SetFullScreen(bool active);

	void SetBorderless(bool active);

	void SetResizable(bool active);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__