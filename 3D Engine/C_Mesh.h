#ifndef __C__Mesh__H__
#define __C__Mesh__H__

#include "Component.h"

class GameObject;

class C_Mesh : public Component

{
public:
	C_Mesh(GameObject* object);
	~C_Mesh();
};

#endif __C__Mesh__H__

