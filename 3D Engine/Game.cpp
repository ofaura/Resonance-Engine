#include "Game.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "FBO.h"
#include "ModuleSceneIntro.h"
#include "C_Mesh.h"

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
		pos = float2(ImGui::GetCursorPosX() + ImGui::GetWindowPos().x, ImGui::GetCursorPosY() + ImGui::GetWindowPos().y);

		posmin.x = ImGui::GetWindowPos().x;
		posmin.y = ImGui::GetWindowPos().y;
		posmax.x = ImGui::GetWindowWidth();
		posmax.y = ImGui::GetWindowHeight();

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
		ImGui::SameLine();
		if (ImGui::Checkbox("Bounding Boxes", &boundingboxes))
		{
			if (!App->scene_intro->ShowBoundingBoxes)
				App->scene_intro->ShowBoundingBoxes = true;
			else
				App->scene_intro->ShowBoundingBoxes = false;
		}
		
		/*ImGui::SameLine();

		if (ImGui::Checkbox("Quad Tree", &quadtree))
		{

			if (!App->scene_intro->ShowQuadtree)
				App->scene_intro->ShowQuadtree = true;
			else
				App->scene_intro->ShowQuadtree = false;

		}
*/
		//
		if (ImGui::ArrowButton("Play", ImGuiDir_Right))
		{
			GameTime = 0;
			if (!App->GameMode)
			{
				//App->Save();
				App->PlayGame();
			}
			else
			{
				//App->Load();
				App->StopPlay();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("||", { 23,19 }) && App->GameMode)
		{
			if (!App->GamePaused)
			{
				App->PauseGame();
			}
			else
			{
				App->ResumeGame();
			}
		}

		ImGui::SameLine();

		if (App->GameMode && !App->GamePaused && step == true)
		{
			App->GamePaused = true;
			step = false;
		}

		if (ImGui::Button("->", { 23,19 }) && App->GameMode && App->GamePaused)
		{
			App->GamePaused = false;
			step = true;
		}
		ImGui::SameLine();

		if (App->GameMode && !App->GamePaused)
			ImGui::Text("Game Timer: On");
		else if (App->GameMode && App->GamePaused)
			ImGui::Text("Game Timer: Paused");
		else
			ImGui::Text("Game Timer: Off");

		ImGui::SameLine();

		char sec[64], min[64];

		float FrameRel = (float)App->maxFPS / (float)App->GameMaxFPS;

		GameTime += App->Game_dt / (FrameRel * FrameRel);

		int nsec = ((int)GameTime) % 60;

		int nmin = ((int)GameTime / 60);

		sprintf(sec, "%02d", nsec);
		sprintf(min, "%02d", nmin);

		std::string smin = min;
		std::string ssec = sec;
		std::string FinalTime = smin + ":" + ssec;

		ImGui::Text(FinalTime.c_str());



		//

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

const ImVec2 Game::GetMouse() const
{
	ImVec2 mousePos = ImGui::GetMousePos();

	ImVec2 realMousePos = ImVec2(mousePos.x - pos.x, mousePos.y - pos.y);
	ImVec2 mouseNormalized;

	mouseNormalized.x = realMousePos.x / size.x;
	mouseNormalized.y = realMousePos.y / size.y;

	mouseNormalized.x = (mouseNormalized.x - 0.5) * 2;
	mouseNormalized.y = (mouseNormalized.y - 0.5) * -2;

	return mouseNormalized;
}

bool Game::isMouseOnScene() const {

	bool ret = ImGui::IsMouseHoveringRect(posmin,posmax);
	return true;
}