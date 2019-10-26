#include "C_Texture.h"

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
			ImGui::Image((void*)texture, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
	}
}
