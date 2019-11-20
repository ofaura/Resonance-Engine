#ifndef __ModuleResourceLoader_H__
#define __ModuleResourceLoader_H__

#include "Module.h"
#include <vector>

#define CHECKERS_WIDTH 150
#define CHECKERS_HEIGHT 150

class C_Mesh;
class C_Texture;
class aiMesh;

// ---------------------------------------------------
class ModuleResourceManager : public Module
{
public:

	ModuleResourceManager(Application* app, bool start_enabled = true);
	virtual ~ModuleResourceManager();


	bool Init();
	bool Start();
	bool CleanUp();

	void LoadFilesFBX(const char* path);
	void LoadMesh(C_Mesh* mesh, aiMesh* currentMesh);
	void LoadMesh(const char* path, C_Mesh* mesh);

	// From Library
	void ImportFile(const char* path);
	bool ImportTexture(const char* path, string& outputFile);
	bool ImportMesh(const char* path, C_Mesh* mesh);

	bool ValidTextureExtension(const string& extension);
	bool ValidMeshExtension(const string& extension);
	bool CheckTextureExtension(const char* extension);
	bool CheckMeshExtension(const char* extension);

	void GenerateTexture(const char* path, C_Texture* texture);
	void GenerateCheckerTexture();

	string GetNameFromPath(string path, bool withExtension = false);

public:

	uint texture;
	uint checker_texture;

	bool loadedAll = false;
};

#endif