#ifndef __ModuleResourceLoader_H__
#define __ModuleResourceLoader_H__

#include "Module.h"
#include <vector>

#define CHECKERS_WIDTH 150
#define CHECKERS_HEIGHT 150



// ---------------------------------------------------
class ModuleResourceManager : public Module
{
public:

	ModuleResourceManager(Application* app, bool start_enabled = true);
	virtual ~ModuleResourceManager();


	bool Init();
	bool Start();
	bool CleanUp();

	void LoadFilesFBX(const char* path, uint tex = 0);
	uint GenerateTexture(const char* path);
	void GenerateCheckerTexture();

public:

	uint texture;
	uint checker_texture;

	bool loadedAll = false;
};

#endif