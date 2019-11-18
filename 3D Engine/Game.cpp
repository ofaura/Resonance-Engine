#include "Game.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "FBO.h"

#include "mmgr/mmgr.h"

Game::Game(bool is_visible) : EditorElement(is_visible) {}

Game::~Game() {}

void Game::Start()
{
}

void Game::Draw()
{
	if (!active) return;
	
	if (ImGui::Begin("Game", &active))
	{
		if (ImGui::Checkbox("Wireframe", &wireframe))
			App->renderer3D->EnableWireframeMode(wireframe);
		ImGui::SameLine();
		if (ImGui::Checkbox("Alpha", &alpha))
			App->renderer3D->EnableAlpha(alpha);
		ImGui::SameLine();
		if (ImGui::Checkbox("Depth Test", &depth_test))
			App->renderer3D->EnableDepthTest(depth_test);
		ImGui::SameLine();
		if (ImGui::Checkbox("Cull Face", &cull_face))
			App->renderer3D->EnableCullFace(cull_face);
		ImGui::SameLine();
		if (ImGui::Checkbox("Lighting", &lighting))
			App->renderer3D->EnableLighting(lighting);
		ImGui::SameLine();
		if (ImGui::Checkbox("Color Material", &color_material))
			App->renderer3D->EnableColorMaterial(color_material);
		ImGui::SameLine();
		if (ImGui::Checkbox("Texture 2D", &texture2D))
			App->renderer3D->EnableTexture2D(texture2D);

		position = ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

		if ((new_size.x != size.x) || (new_size.y != size.y))
		{
			App->renderer3D->OnResize(size.x, size.y);
			new_size = size;
		}

		ImGui::Image((void*)App->renderer3D->fbo_tex->GetTexture(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::End();
}

void Game::CleanUp()
{
}

const ImVec4 Game::GetWindowRect()
{
	return ImVec4(position.x, position.y, size.y, size.x);
}
