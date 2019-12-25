#include "C_AudioListener.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "C_Camera.h"
#include "ModuleSceneIntro.h"

C_AudioListener::C_AudioListener(COMPONENT_TYPE type, GameObject * parent, bool active) : Component(type, parent, active)
{
	name = "AudioListener";

	vec3 position;
	position.x = parent->component_transform->position.x;
	position.y = parent->component_transform->position.y;
	position.z = parent->component_transform->position.z;

	wwiseGO = wwiseGO->CreateAudioListener(parent->UUID, parent->name.c_str(), position);
}

C_AudioListener::~C_AudioListener() {}

void C_AudioListener::DrawInspector()
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_Leaf))
	{

	}
}

void C_AudioListener::Update()
{
	C_Camera* cam = (C_Camera*)App->scene_intro->MainCamera->GetComponent(COMPONENT_TYPE::CAMERA);
	float3 pos = cam->frustum.pos;
	float3 front = cam->frustum.front;
	float3 top = cam->frustum.up;

	wwiseGO->SetPosition(pos.x, pos.y, pos.z, front.x, front.y, front.z, top.x, top.y, top.z);
}

void C_AudioListener::Load(const char * gameObject, const json & file)
{
}

void C_AudioListener::Save(const char * gameObject, json & file)
{
}
