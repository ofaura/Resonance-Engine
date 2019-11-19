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
	ImGui::DragFloat3("Translation", &position.x, 0.001f);
	float3 last_scale = scales;
	ImGui::DragFloat3("Scale", &scales.x, 0.001f);
	Quat last_rotation = rotation;
	ImGui::DragFloat3("Rotation", &rotation.x, 0.1f);

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

	Reset();

}

void C_Transform::Reset()
{
	scales = float3::one;
	position = float3::zero;
	rotation = Quat::identity;

}