#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCES_MANAGER_H__

#include "Module.h"
#include "Math.h"
#include "glmath.h"

#define CHECKERS_WIDTH 10
#define CHECKERS_HEIGHT 10

class Meshes;

class ModuleResourceManager : public Module
{
public:

	ModuleResourceManager( bool start_enabled = true);
	~ModuleResourceManager();

	bool Init(json file);
	bool Start();
	bool CleanUp();
	bool LoadFileFromPath(const char* path);
	void Draw();
	void CheckersTexture(const char* path);

public:

	std::vector<Meshes*> meshes;
	uint tex = 0;

};

#endif