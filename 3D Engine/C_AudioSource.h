#ifndef __C_AudioSource__H__
#define __C_AudioSource__H__

#include "Component.h"
#include <AK/SoundEngine/Common/AkTypes.h>

class WwiseGameObject;

class C_AudioSource : public Component
{
public:
	C_AudioSource(COMPONENT_TYPE type, GameObject* parent, bool active);
	~C_AudioSource();

	void DrawInspector();
	void Update();

	void Load(const char* gameObject, const json &file);
	void Save(const char* gameObject, json &file);

	void SetID(AkGameObjectID id);

public:

	WwiseGameObject* wwiseGO = nullptr;
	AkGameObjectID id = 0;
	bool isPaused = false;
	bool isPlaying = false;
};

#endif __C_AudioSource__H__