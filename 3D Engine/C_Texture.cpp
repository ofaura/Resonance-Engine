#include "C_Texture.h"
#include "ModuleResourceManager.h"
C_Texture::C_Texture(GameObject * object) : Component(COMPONENT_TYPE::TEXTURE, object)
{
	name = "Texture";
}

C_Texture::~C_Texture() {}

void C_Texture::DrawInspector()
{
	if (ImGui::CollapsingHeader("Texture")) 
	{
		ImGui::Checkbox("Enabled ", &active);

		if (active)
		{
			ImGui::SameLine();
			ImGui::Checkbox("Debug Texture", &debug);
			if (debug)
				texture = App->rscr->checker_texture;
			else
			{
				if (original_texture != 0)
					texture = original_texture;
				else
					texture = App->rscr->texture;
			}

			ImGui::Image((void*)texture, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
			ImGui::Image(NULL, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));

	}
}
