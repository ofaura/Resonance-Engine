#ifndef __MODULE_RESOURCE_MANAGER_H__
#define __MODULE_RESOURCES_MANAGER_H__

#include "Module.h"
#include <vector>

struct MeshData {

	uint id_index = 0; // Index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertex = 0; // Vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_tex = 0;  // Texture in VRAM
	uint num_tex = 0;
	float* textures = nullptr;
};

struct Mesh {

	std::vector<MeshData*> mesh;
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

public:
	std::vector<Mesh> MeshArray;
	uint texture;

private:

};

#endif