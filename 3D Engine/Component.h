#ifndef __Component__H__
#define __Component__H__

#include "Globals.h"
#include "Application.h"

#include "imgui-1.72b/imgui.h"
#include "glew/include/GL/glew.h"

class GameObject;

enum class COMPONENT_TYPE
{
	TRANSFORM,
	MESH,
	TEXTURE,
	CAMERA,
	AUDIO_SOURCE,
	AUDIO_LISTENER
};

class Component
{
public:
	Component(COMPONENT_TYPE type, GameObject* parent = nullptr, bool active = true);
	virtual ~Component();

	virtual void Update() {};
	virtual void DrawInspector() {};
	virtual void PostUpdate() {};
	virtual void CleanUp() {};

	void Enable();
	void Disable();

	const COMPONENT_TYPE GetType() const;

	virtual void Load(const char* gameObject, const json &file) {};
	virtual void Save(const char* gameObject, json &file) {};

	uint GetUUID() const;

protected:

	uint UUID = 0;
	uint parentUUID = 0;

public:

	COMPONENT_TYPE type;
	bool active;
	bool open_header = true;
	GameObject* parent;
	string name;

};

#endif __Component__H__
