#ifndef __Configuration__H__
#define __Configuration__H__

#include "EditorManager.h"
#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_version.h"

class Configuration : public EditorElement
{
public:

	Configuration(bool is_visible = true);
	virtual ~Configuration();

public:
	void Start();
	void Draw();
	void CleanUp();

private:
	int		width = SCREEN_WIDTH;
	int		height = SCREEN_HEIGHT;
	float	brightness = 1.0f;

	bool	fullscreen = false;
	bool	resizable = true;
	bool	borderless = false;
	bool	fulldesktop = false;
	
	bool	depth_test = true;
	bool	cull_face = true;
	bool	lighting = true;
	bool	color_material = true;
	bool	texture2D = true;

	GLint video_mem_budget = 0;
	GLint video_mem_available = 0;
	GLint video_mem_usage = 0;

	SDL_version sdl_version;

	string	caps;
	string	cpus;
	string	ram;
	string	gpu;

	char* caps_log;
	char* cpus_log;
	char* ram_log;
	char* gpu_log;
	char* sdl_log;
};

#endif __Configuration__H__
