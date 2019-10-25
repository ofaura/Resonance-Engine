#ifndef __C__Transform__H__
#define __C__Transform__H__

#include "Component.h"
#include "glmath.h"

class GameObject;

class C_Transform : public Component
{
public:
	C_Transform(GameObject* object);
	~C_Transform();

	void Update();

private:
	vec3 position;
	vec3 rotation;
	vec3 scale;

};

#endif __C__Transform__H__

