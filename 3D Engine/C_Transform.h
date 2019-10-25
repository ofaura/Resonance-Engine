#ifndef __C__Transform__H__
#define __C__Transform__H__

#include "Component.h"
#include "glmath.h"

class C_Transform : public Component
{
public:
	C_Transform(GameObject* gameobject, COMPONENT_TYPE type);
	virtual ~C_Transform();

	void Update();

private:
	vec3 position;
	vec3 rotation;
	vec3 scale;

};

#endif __C__Transform__H__

