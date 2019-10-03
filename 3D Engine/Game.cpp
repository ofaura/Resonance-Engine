#include "Game.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "FBO.h"

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
		position = ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		size = ImVec2(ImGui::GetContentRegionAvail().y, ImGui::GetContentRegionAvail().x);

		ImGui::Image((void*)App->renderer3D->fbo_tex->GetTexture(), ImVec2(size.y, size.x), ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::End();
}

void Game::CleanUp()
{
}
