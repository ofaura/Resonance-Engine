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
		/*for (int i = 0; i < App->scene_intro->gameObjects.size(); i++) 
		{
			if (ImGui::Selectable(App->scene_intro->gameObjects[i].name.c_str(), App->scene_intro->id_goSelected == i)) 
			{
				App->scene_intro->id_goSelected = i;
				App->scene_intro->goSelected = &App->scene_intro->gameObjects[i];
			}
		}*/

		vector<GameObject> game_objects = App->scene_intro->gameObjects;
		for (vector<GameObject>::iterator item = game_objects.begin(); item != game_objects.end(); ++item)
		{
			ImGui::PushID((item)->GetId());

			if (ImGui::Selectable((item)->GetName(), (item)->GetSelected()))
			{

				//AddSelectedGameObject((item));
			}
		
			ImGui::PopID();
		}

	}

	ImGui::End();
}

void Hierarchy::CleanUp()
{
}

void Hierarchy::AddSelectedGameObject(GameObject gameObject)
{
	//for (vector<GameObject>::iterator it = selected.begin(); it != selected.end(); ++it)
	//{
	//	if ((it) == gameObject)
	//		return;
	//}

	//gameObject->SetSelected(true);
	//selected.push_back(gameObject);
}

