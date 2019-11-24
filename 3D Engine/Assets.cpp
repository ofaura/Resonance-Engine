#include "Assets.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleResourceManager.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ModuleSceneManager.h"

Assets::Assets(bool is_visible) : EditorElement(is_visible) 
{
	App->fileSystem->DiscoverFiles("Assets", &files, &directories);
	currentDirectory = "Assets";
	parentDirectory = "...";
}

Assets::~Assets() {}

void Assets::Start()
{
}

void Assets::Draw()
{
	files.clear();
	directories.clear();

	App->fileSystem->DiscoverFiles(currentDirectory.data(), &files, &directories);

	if (ImGui::Begin("Assets", &active, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(currentDirectory.data());
		ImGui::Separator();

		if (currentDirectory.compare("Assets") != 0)
		{
			if (ImGui::Selectable("...", false, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseDoubleClicked(0))
					currentDirectory = parentDirectory.data();
			}
		}

		for (int i = 0; i < directories.size(); ++i)
		{
			size_t found = directories[i].find_last_of("/\\");
			string fileName = directories[i].substr(found);
			
			if (ImGui::Selectable(fileName.data(), false, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseDoubleClicked(0))
					currentDirectory = directories[i].data();
			}
		}

		parentDirectory = currentDirectory.substr(0, currentDirectory.find_last_of("/\\"));

		for (int i = 0; i < files.size(); i++)
		{
			size_t found = files[i].find_last_of("/\\");
			string filename = files[i].substr(found + 1);
			if (ImGui::Button(filename.data()))
			{
				string path = currentDirectory + "\\" + filename;

				if (App->rscr->ValidTextureExtension(filename))
				{
					if (App->scene_intro->goSelected != nullptr)
						App->rscr->GenerateTexture(path.c_str(), (C_Texture*)App->scene_intro->goSelected->GetComponent(COMPONENT_TYPE::TEXTURE));
					
					break;
				}

				if (App->rscr->ValidSceneExtension(filename))
				{
					App->scene->LoadScene(App->rscr->GetNameFromPath(filename));
					break;
				}

				App->rscr->FileReceived(path);
			}
		}
	}

	ImGui::End();
}

void Assets::CleanUp()
{
}
