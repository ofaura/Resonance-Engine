#include "C_Transform.h"

#include "mmgr/mmgr.h"

C_Transform::C_Transform(COMPONENT_TYPE type, GameObject * parent, bool active) : Component(type, parent, active)
{
	name = "Transform";
}

C_Transform::~C_Transform()
{
}

void C_Transform::DrawInspector()
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