#include "C_AudioSource.h"
#include "GameObject.h"
#include "C_Transform.h"
#include "ModuleAudio.h"

C_AudioSource::C_AudioSource(COMPONENT_TYPE type, GameObject * parent, bool active) : Component(type, parent, active)
{
	name = "AudioSource";

	vec3 position;
	position.x = parent->component_transform->position.x;
	position.y = parent->component_transform->position.y;
	position.z = parent->component_transform->position.z;

	wwiseGO->CreateAudioSource(parent->UUID, parent->name.c_str(), position);
}

C_AudioSource::~C_AudioSource() {}

void C_AudioSource::DrawInspector()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (id != 0)
		{
			if (ImGui::ArrowButton("Play", ImGuiDir_Right))
				wwiseGO->PlayEvent(id);

			if (ImGui::Button("||"))
				wwiseGO->PauseEvent(id);

			if (ImGui::Button("STOP"))
				wwiseGO->StopEvent(id);
		}
	}
}

void C_AudioSource::Update()
{
	vec3 position;
	position.x = parent->component_transform->position.x;
	position.y = parent->component_transform->position.y;
	position.z = parent->component_transform->position.z;

	wwiseGO->SetPosition(position.x, position.y, position.z);
}

void C_AudioSource::Load(const char * gameObject, const json & file)
{
}

void C_AudioSource::Save(const char * gameObject, json & file)
{
}

void C_AudioSource::SetID(AkGameObjectID id)
{
	this->id = id;
}