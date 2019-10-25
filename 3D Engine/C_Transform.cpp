#include "C_Transform.h"
#include "imgui-1.72b/imgui.h"


C_Transform::C_Transform(GameObject* gameobject, COMPONENT_TYPE type) : Component(type, gameobject)
{
	name = "Transform";
}


C_Transform::~C_Transform()
{
}

void C_Transform::Update()
{

	if (ImGui::CollapsingHeader(name.c_str()))
	{
		//Position

		ImGui::Text("Position:");
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); 
		ImGui::DragFloat("X", &position.x, 0.005f); 
		ImGui::PopID();
		
		ImGui::SameLine(); ImGui::PushItemWidth(60);  
		ImGui::PushID("pos"); 
		ImGui::DragFloat("Y", &position.y, 0.005f); ImGui::PopID();
		
		ImGui::SameLine(); 
		ImGui::PushItemWidth(60);  ImGui::PushID("pos"); 
		ImGui::DragFloat("Z", &position.z, 0.005f); 
		ImGui::PopID();

		//Rotation

		ImGui::Text("Rotation:");
		ImGui::SameLine(); 
		ImGui::PushItemWidth(60); 
		ImGui::PushID("rot"); 
		ImGui::DragFloat("X", &rotation.x, 0.005f); 
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::PushItemWidth(60);
		ImGui::PushID("rot");  
		ImGui::DragFloat("Y", &rotation.y, 0.005f); 
		ImGui::PopID();

		ImGui::SameLine(); 
		ImGui::PushItemWidth(60); 
		ImGui::PushID("rot");  
		ImGui::DragFloat("Z", &rotation.z, 0.005f); 
		ImGui::PopID();

		//Scale

		ImGui::Text("Scale:   ");
		ImGui::SameLine(); 
		ImGui::PushItemWidth(60);  
		ImGui::PushID("scale"); 
		ImGui::DragFloat("X", &scale.x, 0.005f); 
		ImGui::PopID();

		ImGui::SameLine(); 
		ImGui::PushItemWidth(60);  
		ImGui::PushID("scale"); 
		ImGui::DragFloat("Y", &scale.y, 0.005f);
		ImGui::PopID();

		ImGui::SameLine();
		ImGui::PushItemWidth(60);
		ImGui::PushID("scale"); 
		ImGui::DragFloat("Z", &scale.z, 0.005f); 
		ImGui::PopID();
	}

		ImGui::GetStateStorage()->SetInt(ImGui::GetID(name.c_str()), 1);
}