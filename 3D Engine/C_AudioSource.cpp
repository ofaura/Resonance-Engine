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

	wwiseGO = wwiseGO->CreateAudioSource(parent->UUID, parent->name.c_str(), position);
}

C_AudioSource::~C_AudioSource() {}

void C_AudioSource::DrawInspector()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (id != 0)
		{
			string state = "Pause";

			if (ImGui::ArrowButton("Play", ImGuiDir_Right))
			{
				if (!isPlaying)
				{
					wwiseGO->PlayEvent(id);
					isPlaying = true;
				}
				isPaused = false;
			}
			
			ImGui::SameLine();
	
			
			if (isPaused)
				state = "Resume";

			else
				state = "Pause";

			if (ImGui::Button(state.c_str()))
			{
				if (isPlaying)
				{
					isPlaying = false;
					isPaused = true;
					wwiseGO->PauseEvent(id);
				}
				else if (isPaused)
				{
					isPlaying = true;
					isPaused = false;
					wwiseGO->ResumeEvent(id);
				} 
			}

			ImGui::SameLine();

			if (ImGui::Button("STOP"))
			{
				isPlaying = false;
				isPaused = false;
				wwiseGO->StopEvent(id);
			}

			if (ImGui::SliderFloat("Volume", &wwiseGO->volume, 0, 10))
			{
				wwiseGO->SetVolume(id, wwiseGO->volume);
			}
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