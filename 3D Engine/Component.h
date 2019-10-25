#ifndef __Component__H__
#define __Component__H__
#include "Globals.h"
#include "Application.h"

class GameObject;

enum class COMPONENT_TYPE
{
	TRANSFORM,
	MESH,
	MATERIAL
};

class Component
{
public:
	Component(COMPONENT_TYPE type, GameObject* gameobject);
	virtual ~Component();

	virtual void Update() {};

	void Enable();
	void Disable();

	const COMPONENT_TYPE GetType();

protected:

	COMPONENT_TYPE type;
	bool active = false;
	GameObject* my_go;
	string name;

};

#endif __Component__H__
