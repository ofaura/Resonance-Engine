#ifndef __Application__
#define __Application__

#include "Globals.h"
#include "Timer.h"
#include "Module.h"

#include <list>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class EditorManager;


class Application
{
public:
	ModuleWindow*			window = nullptr;
	ModuleInput*			input = nullptr;
	ModuleSceneIntro*		scene_intro = nullptr;
	ModuleRenderer3D*		renderer3D = nullptr;
	ModuleCamera3D*			camera = nullptr;
	EditorManager*			editor = nullptr;

private:

	uint					frame_count = 0;
	Timer					startup_time;
	Timer					frame_time;
	Timer					last_sec_frame_time;
	uint					last_sec_frame_count = 0;
	uint					prev_last_sec_frame_count = 0;
	uint					framerate_cap = 0;
	uint					maxFPS = 60;

	float	dt;
	

	list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	uint GetFramerateLimit() const;
	void SetFramerateLimit(uint max_framerate);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif __Application__