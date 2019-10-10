#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCES_MANAGER_H__

#include "Module.h"
#include "Math.h"
#include "glmath.h"

class Meshes;

class ModuleResourceManager : public Module
{
public:

	ModuleResourceManager( bool start_enabled = true);
	~ModuleResourceManager();

	bool Init(json file);
	bool CleanUp();
	bool LoadFileFromPath(const char* path);
	void Draw();

private:

	std::vector<Meshes*> meshes;
};

#endif