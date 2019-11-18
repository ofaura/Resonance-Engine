#include "C_Texture.h"
#include "ModuleResourceManager.h"
#include "ModuleSceneIntro.h"

#include "mmgr/mmgr.h"


C_Texture::C_Texture(COMPONENT_TYPE type, GameObject * parent, bool active) : Component(type, parent, active)
{
}

C_Texture::~C_Texture() {}

void C_Texture::DrawInspector()
{
	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Enabled ", &active);

		if (active)
		{
			ImGui::SameLine();
			ImGui::Checkbox("Debug Texture", &debug);

			ImGui::Text(texture_path.c_str());

			if (debug)
				texture = App->rscr->checker_texture;
			else
			{
				if (original_texture != 0)
					texture = original_texture;
				//else  if (App->scene_intro->id_goSelected <= 1)
				//	texture = App->rscr->texture;
				else
					texture = NULL;
			}

			ImGui::Image((void*)texture, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
			ImGui::Image(NULL, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));

	}
}
