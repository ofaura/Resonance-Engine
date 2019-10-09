#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCES_MANAGER_H__

#include "Module.h"
#include "Math.h"
#include "glmath.h"

class ModuleResourceManager : public Module
{
public:

	ModuleResourceManager( bool start_enabled = true);
	~ModuleResourceManager();

	bool Init(json file);
	bool Start();
	bool CleanUp();
	
	void Draw();
	bool LoadFileFromPath(const char* path);

private:

	vec3* Vertices = nullptr;
	uint VerticesID = 0; // unique vertex in VRAM
	uint verticesSize = 0;

	uint* Indices = nullptr;
	uint IndicesID = 0; // index in VRAM
	uint IndicesSize = 0;
};

#endif