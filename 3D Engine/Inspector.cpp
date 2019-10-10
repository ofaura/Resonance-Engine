#include "Inspector.h"
#include "Application.h"
#include "ModuleWindow.h"

Inspector::Inspector(bool is_visible) : EditorElement(is_visible) {}

Inspector::~Inspector() {}

void Inspector::Start()
{
}

void Inspector::Draw()
{
	if (!active) return;
	if (ImGui::Begin("Inspector", &active, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("GameObject Info");
	}

	ImGui::End();
}

void Inspector::CleanUp()
{
}
