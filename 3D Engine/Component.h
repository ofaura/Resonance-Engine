#ifndef __Component__H__
#define __Component__H__

#include "Globals.h"
#include "Application.h"

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
	Component(COMPONENT_TYPE type, GameObject* gameobject);
	virtual ~Component();

	virtual void Update() {};
	//virtual void DrawInspector();

	void Enable();
	void Disable();

	const COMPONENT_TYPE GetType() const;

protected:

	COMPONENT_TYPE type;
	bool active = false;
	GameObject* my_go;
	string name;

};

#endif __Component__H__
