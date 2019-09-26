#include "Hierarchy.h"
#include "Application.h"
#include "ModuleWindow.h"

Hierarchy::Hierarchy() {}

Hierarchy::~Hierarchy() {}

void Hierarchy::Start()
{
}

void Hierarchy::Draw()
{
	App->window->GetWindowSize(window_width, window_height);
	ImGui::SetNextWindowPos(ImVec2(0, 18), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)window_width / 5, (float)window_height - 18), ImGuiCond_Once);

	if (ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
	{			

		ImGui::End();
	}
}

void Hierarchy::CleanUp()
{
}
