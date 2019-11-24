#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "EditorManager.h"
#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneManager.h"

#include "mmgr/mmgr.h"

Application::Application()
{

	randomNumber = new LCG();

	window = new ModuleWindow();
	input = new ModuleInput();
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	editor = new EditorManager();
	rscr = new ModuleResourceManager();
	fileSystem = new ModuleFileSystem();
	scene = new ModuleSceneManager();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);	
	AddModule(editor);
	AddModule(fileSystem);
	AddModule(scene);
	AddModule(rscr);

	// Scenes
	AddModule(scene_intro);
	
	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	list<Module*>::iterator item = list_modules.begin();

	for (; item != list_modules.end(); item = next(item))
	{
		RELEASE(*item);
	}

	list_modules.clear();

	if (randomNumber)
		RELEASE(randomNumber);
}

bool Application::Init()
{
	bool ret = true;

	json config = jsonLoader.Load("Configuration.json");

	// Call Init() in all modules
	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->Init(config);
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->Start();
	}

	frame_time.Start();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = (float)frame_time.ReadSec();
	frame_time.Start();



	if (!GameMode || GamePaused)
		Game_dt = 0.0f;
	else
		Game_dt = dt * GameSpeed;

	if (GameMode && !StartCount)
	{
		StartCount = true;
		GameTimer.Start();
	}
	//Game_dt *= GameSpeed;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}
	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint last_frame_ms = frame_time.Read();
	uint frames_on_last_update = prev_last_sec_frame_count;

	// Update the fps vector
	fps_vec.push_back(prev_last_sec_frame_count);
	if (fps_vec.size() > HISTOGRAM_BARS)
		fps_vec.erase(fps_vec.begin());
	
	// Update the ms vector
	ms_vec.push_back(last_frame_ms);
	if (ms_vec.size() > HISTOGRAM_BARS)
		ms_vec.erase(ms_vec.begin());
	
	if (maxFPS > 0)
		capped_ms = 1000 / maxFPS;
	else
		capped_ms = 0;

	if (capped_ms > 0 && last_frame_ms < capped_ms)
		SDL_Delay(capped_ms - last_frame_ms);
}

void Application::LoadAllConfig(json &file)
{
	json config = jsonLoader.Load("Configuration.json");

	for (list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		(*item)->LoadConfig(config);
	}
}

void Application::SaveAllConfig()
{
	json config = {
		{"Application", 
			{
				{"Title", "Resonance Engine"}
			}
		}
	};

	for (list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		(*item)->SaveConfig(config);
	}

	jsonLoader.Save("Configuration.json", config);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; ++item) 
	{
		ret = (*item)->PreUpdate(dt);
	}

	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->Update(dt);
	}
	
	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->PostUpdate(dt);
	}
	

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	SaveAllConfig();

	for (list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend() && ret; ++item)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

LCG & Application::GetRandom()
{
	int a = (int)randomNumber;
	return *randomNumber;
}

