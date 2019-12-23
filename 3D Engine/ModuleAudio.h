#ifndef __ModuleAudio__H__
#define __ModuleAudio__H__

#include "Module.h"
#include "glmath.h"
#include <AK/SoundEngine/Common/AkTypes.h>

class WwiseGameObject
{
	WwiseGameObject(unsigned __int64 id, const char* name);
	~WwiseGameObject();

private:
	void SetPosition(vec3 pos, vec3 front, vec3 top);
	void PlayEvent(uint id);
	void PauseEvent(uint id);
	void ResumeEvent(uint id);
	void StopEvent(uint id);
	

private:
	const char* name = nullptr;
	AkGameObjectID id;

	AkVector position;
	AkVector orientationFront;
	AkVector orientationTop;
};

class ModuleAudio :	public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	virtual ~ModuleAudio();

	bool Start();
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	
	void InitWwise();
	void TerminateWwise();


};

#endif __ModuleAudio__H__
