#include "ModuleSceneManager.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"
#include "ModuleWindow.h"
#include "C_Camera.h"

#include <fstream>
#include <iomanip>

#include "mmgr/mmgr.h"

ModuleSceneManager::ModuleSceneManager(bool start_enabled) : Module("Scene Manager", start_enabled)
{
}


ModuleSceneManager::~ModuleSceneManager()
{
}

update_status ModuleSceneManager::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || saveScene)
	{
		ImGui::OpenPopup("Save scene");
	}

	SaveScenePopUp();

	return UPDATE_CONTINUE;
}

bool ModuleSceneManager::Start()
{
	SetSceneName(sceneName);

	return true;
}

void ModuleSceneManager::LoadScene(const string scene)
{
	sceneName = scene;

	CleanUp();

	json file;
	string path = ASSETS_SCENE_FOLDER + scene + sceneExtension;

	ifstream stream;
	stream.open(path);
	file = json::parse(stream);
	stream.close();

	App->scene_intro->root = new GameObject("root", nullptr);

	numGO = file["Game Objects"]["Count"];
	LoadAllGO(App->scene_intro->root, file);
	
	App->scene_intro->MainCamera = new GameObject("Main Camera", App->scene_intro->root);
	App->scene_intro->MainCamera->AddComponent(COMPONENT_TYPE::CAMERA);

	App->scene_intro->goSelected = nullptr;
	goLoaded = 0;

	SetSceneName(sceneName);
}

void ModuleSceneManager::SaveScene(string scene)
{
	SetSceneName(sceneName);

	json file;
	string path = ASSETS_SCENE_FOLDER + scene + sceneExtension;

	SaveAllGO(App->scene_intro->root, file);
	file["Game Objects"]["Count"] = goSaved;
	goSaved = 0;

	ofstream stream;
	stream.open(path);
	stream << setw(4) << file << endl;
	stream.close();
}

string ModuleSceneManager::GetSceneName()
{
	return sceneName;
}

void ModuleSceneManager::LoadAllGO(GameObject * root, const json & file)
{
	goLoaded++;
	char name[50];
	sprintf_s(name, 50, "GameObject %d", goLoaded);
	root->Load(name, file);

	for (int i = 1; i <= numGO; ++i)
	{
		char name[50];
		sprintf_s(name, 50, "GameObject %d", i);

		uint tmp = file["Game Objects"][name]["Parent UUID"];

		if (tmp == root->GetId() && i != 1)
		{
			loadingScene = true;
			GameObject* go = new GameObject("", root);
			loadingScene = false;
		}
	}

	for (int i = 0; i < root->children.size(); ++i)
		LoadAllGO(root->children[i], file);
}

void ModuleSceneManager::SaveAllGO(GameObject * root, json & file)
{
	goSaved++;
	char name[50];
	sprintf_s(name, 50, "GameObject %d", goSaved);
	root->Save(name, file);

	for (int i = 0; i < root->children.size(); ++i)
	{
		if (root->children[i]->name != "Main Camera")
			SaveAllGO(root->children[i], file);
	}
}

void ModuleSceneManager::DeleteSceneGO(GameObject * root)
{
	for (int i = 0; i < root->children.size(); ++i)
	{
		root->children[i]->CleanUp();
		DeleteSceneGO(root->children[i]);		
	}

	RELEASE(root);
}

void ModuleSceneManager::SetSceneName(string name)
{
	string title = TITLE;
	title += "         \t";
	title += name;
	title += sceneExtension;

	SDL_SetWindowTitle(App->window->window, title.c_str());
}

void ModuleSceneManager::SaveScenePopUp()
{
	if (ImGui::BeginPopupModal("Save scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char name[100] = "";
		strcpy_s(name, 100, sceneName.c_str());
		
		if (ImGui::InputText("", name, 100, ImGuiInputTextFlags_AutoSelectAll))
			sceneName = name;
		
		ImGui::NewLine();
		ImGui::Separator();

		if (ImGui::Button("Save", ImVec2(140, 0)))
		{
			SaveScene(sceneName);
			ImGui::CloseCurrentPopup();
			saveScene = false;
		}
		
		ImGui::SameLine();
		
		if (ImGui::Button("Cancel", ImVec2(140, 0)))
		{
			saveScene = false;
			ImGui::CloseCurrentPopup(); 
		}
		
		ImGui::EndPopup();
	}
}

void ModuleSceneManager::LoadScenePopUp()
{
	sceneList.clear();
	App->fileSystem->GetAllFilesWithExtension(ASSETS_SCENE_FOLDER, "scene", sceneList);

	for (uint i = 0; i < sceneList.size(); i++)
	{
		if (ImGui::MenuItem(sceneList[i].c_str()))
		{
			SaveScene(sceneName);
			LoadScene(GetNameFromPath(sceneList[i].c_str()));
		}
	}
}

bool ModuleSceneManager::CleanUp()
{
	DeleteSceneGO(App->scene_intro->root);
	numGO = 0;

	return true;
}

string ModuleSceneManager::GetNameFromPath(string path, bool withExtension)
{
	string name;

	App->fileSystem->NormalizePath(path);
	name = path.substr(path.find_last_of("//") + 1);

	if (withExtension)
		return name;

	else
	{
		string::size_type const p(name.find_last_of('.'));
		string file_name = name.substr(0, p);

		return file_name;
	}
}
