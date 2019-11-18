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

	ImGui::DragFloat3("Translation", &vposition.x, 0.05f);
	ImGui::DragFloat3("Scale", &vscale.x, 0.05f);
	ImGui::DragFloat3("Rotation", &vrotation.x, 0.5f);
	
}

void C_Transform::UpdateMatrices() {

	mat4x4 translation = translate(vposition.x, vposition.y, vposition.z);
	mat4x4 rotation, aux;
	rotation = rotation * aux.rotate(vrotation.x, { 1,0,0 });
	rotation = rotation * aux.rotate(vrotation.y, { 0,1,0 });
	rotation = rotation * aux.rotate(vrotation.z, { 0,0,1 });

	mat4x4 scaling = scale(vscale.x, vscale.y, vscale.z);

	localMatrix = translation * rotation * scaling;

	if (parent != nullptr)
		globalMatrix = parent->component_transform->globalMatrix * localMatrix;
	else
		globalMatrix = localMatrix;

	for (int i = 0; i < parent->children.size(); ++i) {
		parent->children[i]->component_transform->UpdateMatrices();
	}
}