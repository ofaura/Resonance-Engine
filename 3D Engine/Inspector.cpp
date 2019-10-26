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
		//ImGui::Text(App->scene_intro->goSelected->name.c_str());
		App->scene_intro->goSelected->DrawInspector();
	}
	
	ImGui::End();
}

void Inspector::CleanUp()
{
}
