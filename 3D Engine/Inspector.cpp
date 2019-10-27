#include "Inspector.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"

Inspector::Inspector(bool is_visible) : EditorElement(is_visible) {}

Inspector::~Inspector() {}

void Inspector::Start()
{
}

void Inspector::Draw()
{
	if (!active) return;
	ImGui::Begin("Inspector", &active, ImGuiWindowFlags_AlwaysAutoResize);
	
	if (App->scene_intro->goSelected != nullptr)
	{
		GameObject* Selected = App->scene_intro->gameObjects.at(App->scene_intro->id_goSelected);

		ImGui::BeginChild("child", ImVec2(0, 35), true);

		ImGui::Checkbox(" ", &App->scene_intro->goSelected->enable);
		ImGui::SameLine();

		static char name[100] = "";
		strcpy_s(name, 100, Selected->GetName());
		if (ImGui::InputText("", name, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			Selected->SetName(name);
			changedName = true;
		}

		if (changedName)
		{ 
			int count = 0;
			string num;
			for (uint k = 0; k < App->scene_intro->gameObjects.size(); ++k)
			{
				if (App->scene_intro->gameObjects.at(k)->name == name && k != App->scene_intro->id_goSelected)
				{
					count++;
					num = std::to_string(count);
					strcat_s(name, " (");
					strcat_s(name, num.c_str());
					strcat_s(name, ")");
				}
			}
			Selected->SetName(name);
			changedName = false;
		}


		ImGui::EndChild();

		App->scene_intro->goSelected->DrawInspector();
	}
	
	ImGui::End();
}

void Inspector::CleanUp()
{
}
