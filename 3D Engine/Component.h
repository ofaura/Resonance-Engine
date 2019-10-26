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
	TEXTURE
};

class Component
{
public:
	Component(COMPONENT_TYPE type, GameObject* parent = nullptr, bool active = true);
	virtual ~Component();

	virtual void Update() {};
	virtual void DrawInspector() {};

	void Enable();
	void Disable();

	const COMPONENT_TYPE GetType() const;

public:

	COMPONENT_TYPE type;
	bool active;
	GameObject* parent;
	string name;

};

#endif __Component__H__
