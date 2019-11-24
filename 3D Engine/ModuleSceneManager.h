#ifndef __ModuleSceneManager__H__
#define __ModuleSceneManager__H__

#include "Globals.h"
#include "Module.h"

#include <vector>

class GameObject;

class ModuleSceneManager : public Module
{
public:
	ModuleSceneManager(bool start_enabled = true);
	~ModuleSceneManager();

	update_status Update(float dt);
	update_status PostUpdate(float dt) { return UPDATE_CONTINUE; };

	void SaveScenePopUp();
	void LoadScenePopUp();

	void SaveScene(string scene);

	string GetSceneName();

private:

	bool Start();

	void LoadScene(const string scene);
	void LoadAllGO(GameObject* root, const json &file);
	void SaveAllGO(GameObject* root, json &file);
	
	void DeleteSceneGO(GameObject* root);

	void SetSceneName(string name);

	bool CleanUp();

	string GetNameFromPath(string path, bool withExtension = false);

private:

	uint numGO = 0;
	uint goLoaded = 0;
	uint goSaved = 0;

	const char* sceneExtension = ".json";

	string sceneName = "Untitled";

	vector<string> sceneList;

public:

	bool loadingScene = false;
	bool saveScene = false;
};

#endif __ModuleSceneManager__H__
