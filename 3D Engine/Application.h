#ifndef __Application__
#define __Application__

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"

#include <list>

#define HISTOGRAM_BARS 100

class Module;
class ModuleFileSystem;
class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class EditorManager;
class ModuleResourceManager;
class ModuleSceneManager;
class ModuleAudio;

class Application
{
public:
	ModuleWindow*			window = nullptr;
	ModuleInput*			input = nullptr;
	ModuleSceneIntro*		scene_intro = nullptr;
	ModuleRenderer3D*		renderer3D = nullptr;
	ModuleCamera3D*			camera = nullptr;
	EditorManager*			editor = nullptr;
	ModuleResourceManager*  rscr = nullptr;
	ModuleFileSystem*		fileSystem = nullptr;
	ModuleSceneManager*		scene = nullptr;
	ModuleAudio*			audio = nullptr;

private:

	JSONLoader				jsonLoader;

	uint					frame_count = 0;
	Timer					startup_time;
	Timer					frame_time;
	Timer					last_sec_frame_time;
	uint					last_sec_frame_count = 0;
	uint					prev_last_sec_frame_count = 0;
	uint					framerate_cap = 0;
	int						capped_ms = -1;

	float	dt;


	list<Module*> list_modules;

public:
	
	int maxFPS = 60;
	vector<float> fps_vec;
	vector<float> ms_vec;
	list<char*> log_list;
	LCG* randomNumber = nullptr;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	LCG& GetRandom();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
	void LoadAllConfig(json &file);
	void SaveAllConfig();

public:

	float Game_dt = 0.0f;
	float GameSpeed = 1.0f;
	int GameMaxFPS = 60;
	bool GameMode = false;
	bool GamePaused = false;

	void PlayGame();
	void PauseGame();
	void ResumeGame();
	void StopPlay();

};

extern Application* App;

#endif __Application__