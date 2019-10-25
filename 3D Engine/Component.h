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
	Component(COMPONENT_TYPE type);
	virtual ~Component();

	virtual void Start() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	void Enable();
	void Disable();

	const COMPONENT_TYPE GetType();

private:

	COMPONENT_TYPE type;
	bool active = false;
	GameObject* my_go;
	string name;

};

#endif __Component__H__
