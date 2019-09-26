#ifndef __Configuration__H__
#define __Configuration__H__

#include "EditorManager.h"

class Configuration : public EditorElement
{
public:

	Configuration();
	virtual ~Configuration();

public:
	void Start();
	void Draw();
	void CleanUp();

private:
	int		window_width = 0;
	int		window_height = 0;
	int		width = SCREEN_WIDTH;
	int		height = SCREEN_HEIGHT;
	float	brightness = 1.0f;

	bool	fullscreen = false;
	bool	resizable = true;
	bool	borderless = false;
	bool	fulldesktop = false;

	string	caps;
	string	cpus;
	string	ram;
	string	gpu;
};

#endif __Configuration__H__
