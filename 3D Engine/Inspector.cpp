#include "Inspector.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

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
		GameObject* Selected = App->scene_intro->goSelected;

		ImGui::BeginChild("child", ImVec2(0, 35), true);

		ImGui::Checkbox(" ", &App->scene_intro->goSelected->enable);
		ImGui::SameLine();

		static char name[100] = "";
		strcpy_s(name, 100, Selected->GetName());
		if (ImGui::InputText("", name, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			//Selected->SetName(name);
			changedName = true;
		}

		if (changedName)
		{ 
			int count = 0;
			string final_name = name;
			string num;
			for (int i = 0; i < App->scene_intro->root->children.size(); i++)
			{
				if (App->scene_intro->root->children[i] != Selected)
				{
					if (strcmp(App->scene_intro->root->children[i]->name.c_str(), name) == 0)
					{	
						count++;
						//final_name = strcat(name, "(");
						//num = std::to_string(count);
						//final_name = strcat_s(name, num.c_str());
						//final_name = strcat(name, ")");
					}
				}

			}

			if (count != 0)
			{
				final_name = strcat(name, "(");
				num = std::to_string(count);
				final_name = strcat_s(name, num.c_str());
				final_name = strcat(name, ")");
			}

			//name = App->scene_intro->SetAvailableName(name);
			Selected->SetName(final_name.c_str());
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
