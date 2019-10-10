#include "Hierarchy.h"
#include "Application.h"
#include "ModuleWindow.h"

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
		if (ImGui::MenuItem("GameObject"))
		{

		}
	}

	ImGui::End();
}

void Hierarchy::CleanUp()
{
}
