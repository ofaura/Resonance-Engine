#include "C_Transform.h"
#include "GameObject.h"

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
	float3 last_position = position;
	ImGui::DragFloat3("Translation", &position.x, 0.05f);
	float3 last_scale = scales;
	ImGui::DragFloat3("Scale", &scales.x, 0.05f);
	Quat last_rotation = rotation;
	ImGui::DragFloat3("Rotation", &rotation.x, 0.5f);

	if (last_position.x != position.x || last_position.y != position.y || last_position.z != position.z ||last_rotation.x != rotation.x || last_rotation.y != rotation.y || last_rotation.z != rotation.z || last_scale.x != scales.x || last_scale.y != scales.y || last_scale.z != scales.z)
	{
		UpdateMatrix(); 
	}
	
}

void C_Transform::UpdateMatrix() {

	mat4x4 translation = translate(position.x, position.y, position.z);
	mat4x4 scalate = scale(scales.x, scales.y, scales.z);
	mat4x4 AuxRotation;
	mat4x4 auxiliar;

	AuxRotation = AuxRotation * auxiliar.rotate(rotation.x, { 1,0,0 });
	AuxRotation = AuxRotation * auxiliar.rotate(rotation.y, { 0,1,0 });
	AuxRotation = AuxRotation * auxiliar.rotate(rotation.z, { 0,0,1 });
	localMatrix = translation * AuxRotation * scalate;

	if (parent != nullptr) { globalMatrix = parent->component_transform->globalMatrix * localMatrix; }	
	else { globalMatrix = localMatrix; }

	for (int i = 0; i < parent->children.size(); ++i) {
		parent->children[i]->component_transform->UpdateMatrix();
	}
}

void C_Transform::Load(const char * gameObject, const json & file)
{
	UUID = file["Game Objects"][gameObject]["Components"]["Transform"]["UUID"];
	parentUUID = file["Game Objects"][gameObject]["Components"]["Transform"]["Parent UUID"];
	active = file["Game Objects"][gameObject]["Components"]["Transform"]["Active"];

	rotation.x = file["Game Objects"][gameObject]["Components"]["Transform"]["Rotation"].at(0);
	rotation.y = file["Game Objects"][gameObject]["Components"]["Transform"]["Rotation"].at(1);
	rotation.z = file["Game Objects"][gameObject]["Components"]["Transform"]["Rotation"].at(2);
	rotation.w = file["Game Objects"][gameObject]["Components"]["Transform"]["Rotation"].at(3);

	position.x = file["Game Objects"][gameObject]["Components"]["Transform"]["Position"].at(0);
	position.y = file["Game Objects"][gameObject]["Components"]["Transform"]["Position"].at(1);
	position.z = file["Game Objects"][gameObject]["Components"]["Transform"]["Position"].at(2);

	scales.x = file["Game Objects"][gameObject]["Components"]["Transform"]["Scale"].at(0);
	scales.y = file["Game Objects"][gameObject]["Components"]["Transform"]["Scale"].at(1);
	scales.z = file["Game Objects"][gameObject]["Components"]["Transform"]["Scale"].at(2);

}

void C_Transform::Save(const char * gameObject, json & file)
{
	file["Game Objects"][gameObject]["Components"]["Transform"]["UUID"] = UUID;
	file["Game Objects"][gameObject]["Components"]["Transform"]["Parent UUID"] = parentUUID;
	file["Game Objects"][gameObject]["Components"]["Transform"]["Active"] = active;

	file["Game Objects"][gameObject]["Components"]["Transform"]["Rotation"] = { rotation.x, rotation.y, rotation.z, rotation.w };
	file["Game Objects"][gameObject]["Components"]["Transform"]["Position"] = { position.x, position.y, position.z };
	file["Game Objects"][gameObject]["Components"]["Transform"]["Scale"] = { scales.x, scales.y, scales.y };
}
