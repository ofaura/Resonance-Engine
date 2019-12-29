#ifndef __ModuleAudio__H__
#define __ModuleAudio__H__

#include "Module.h"
#include "glmath.h"
#include <AK/SoundEngine/Common/AkTypes.h>

class WwiseGameObject
{
	WwiseGameObject(unsigned __int64 id, const char* name);
	~WwiseGameObject();

public:
	void SetPosition(float posX = 0, float posY = 0, float posZ = 0, float frontX = 1, float frontY = 0, float frontZ = 0, float topX = 0, float topY = 1, float topZ = 0);
	void PlayEvent(uint id);
	void PauseEvent(uint id);
	void ResumeEvent(uint id);
	void StopEvent(uint id);
	void SetVolume(uint id, float volume);
	WwiseGameObject* CreateAudioSource(uint id, const char* name, vec3 position);
	WwiseGameObject* CreateAudioListener(uint id, const char* name, vec3 position);
	void SetAuxSends();

public:
	uint GetID();

public:
	float volume = 5.0f;

private:
	const char* name = nullptr;
	AkGameObjectID id;

	AkVector position = { 0,0,0 };
	AkVector orientationFront = { 0,0,0 };
	AkVector orientationTop = { 0,0,0 };
};

class ModuleAudio : public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	virtual ~ModuleAudio();

	bool Init(json file);
	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();
	void Tests(AkGameObjectID id);

private:

	void InitWwise();
	void TerminateWwise();
	void LoadSoundBank(const char* path);

public:
	AkGameObjectID currentListenerID;
};

#endif __ModuleAudio__H__
