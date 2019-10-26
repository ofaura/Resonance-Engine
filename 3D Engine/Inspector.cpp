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

		static char GOName[100] = "";
		strcpy_s(GOName, 100, Selected->GetName());
		if (ImGui::InputText("", GOName, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			Selected->SetName(GOName);

		ImGui::EndChild();

		App->scene_intro->goSelected->DrawInspector();
	}
	
	ImGui::End();
}

void Inspector::CleanUp()
{
}
