#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCES_MANAGER_H__

#include "Module.h"
#include <vector>

#define CHECKERS_WIDTH 100
#define CHECKERS_HEIGHT 100

struct Data {

	uint id_index = 0; 
	uint num_indices = 0;
	uint* indices = nullptr;
	uint id_vertex = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;
	uint id_texture = 0;
	uint num_texture = 0;
	float* textures = nullptr;
};

struct Mesh {

	std::vector<Data*> mesh;
	uint texture = 0;
};

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
	void Draw(Mesh fbx_mesh);
	uint GenerateTexture(const char* path);
	void GenerateCheckerTexture();

public:
	
	std::vector<Mesh> MeshArray;
	uint texture;
	uint checker_texture;

};

#endif