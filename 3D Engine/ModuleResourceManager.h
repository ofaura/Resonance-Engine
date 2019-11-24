#ifndef __ModuleResourceLoader_H__
#define __ModuleResourceLoader_H__

#include "Module.h"
#include <vector>

#define CHECKERS_WIDTH 150
#define CHECKERS_HEIGHT 150

class GameObject;
class C_Mesh;
class C_Texture;
struct aiMesh;
struct aiScene;
struct aiNode;
struct aiMaterial;
struct ILinfo;
class mat4x4;

// ---------------------------------------------------
class ModuleResourceManager : public Module
{
public:

	ModuleResourceManager(bool start_enabled = true);
	virtual ~ModuleResourceManager();


	bool Init();
	bool Start();
	bool CleanUp();

	bool FileReceived(string path);
	
	GameObject* LoadFilesFBX(aiNode* node, const aiScene* scene, string* FileName, GameObject* parent);

	void LoadMesh(C_Mesh* mesh, aiMesh* currentMesh);
	void LoadMesh(const char* path, C_Mesh* mesh);

	// From Library
	void ImportFile(string * path);
	bool ImportTexture(const char* path, string& outputFile, C_Texture* texture = nullptr);
	bool ImportMesh(const char* path, C_Mesh* mesh);

	bool ValidTextureExtension(const string& extension);
	bool ValidMeshExtension(const string& extension);
	bool ValidSceneExtension(const string& extension);

	bool CheckTextureExtension(const char* extension);
	bool CheckMeshExtension(const char* extension);

	void GenerateTexture(const char* path, C_Texture* texture);
	void GenerateCheckerTexture();

	string GetNameFromPath(string path, bool withExtension = false);
	string* RemoveSpacesFromPath(string* path);

public:

	uint texture;
	uint checker_texture;
};

#endif