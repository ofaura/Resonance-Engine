#include "Hierarchy.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"

Hierarchy::Hierarchy(bool is_visible) : EditorElement(is_visible) {}

Hierarchy::~Hierarchy() {}

void Hierarchy::Start()
{
}

void Hierarchy::Draw()
{
	if (!active) return;
	if (ImGui::Begin("Hierarchy", &active, ImGuiWindowFlags_AlwaysAutoResize))
	{			
		ImGui::Text("a");
		/*for (int i = 0; i < App->scene_intro->gameObjects.size(); i++) 
		{
			if (ImGui::Selectable(App->scene_intro->gameObjects[i].name.c_str(), App->scene_intro->id_goSelected == i)) 
			{
				App->scene_intro->id_goSelected = i;
				App->scene_intro->goSelected = &App->scene_intro->gameObjects[i];
			}
		}*/

		//for (uint i = 0; i < App->scene_intro->gameObjects.size(); i++)
		//{
		//	//if (ImGui::Selectable(App->scene_intro->gameObjects.at(i)->name.c_str(), App->scene_intro->id_goSelected == i))
		//	//{
		//	//	App->scene_intro->id_goSelected = i;
		//	//	//App->scene_intro->goSelected = App->scene_intro->gameObjects.at(i);
		//	//}
		//}
	}

	ImGui::End();
}

void Hierarchy::CleanUp()
{
}
