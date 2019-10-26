#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCE_MANAGER_H__

#define CHECKERS_WIDTH 100
#define CHECKERS_HEIGHT 100

#include "Module.h"

#include <vector>

// ---------------------------------------------------
class ModuleResourceManager : public Module
{
public:

	ModuleResourceManager(Application* app, bool start_enabled = true);
	virtual ~ModuleResourceManager();


	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LoadFilesFromPath(const char* path, uint tex = 0);
	uint GenerateTexture(const char* path);
	void GenerateCheckerTexture();


public:
	
	uint texture;
	uint checker_texture;

	bool loadedAll = false;
};

#endif