#ifndef __C__Texture__H__
#define __C__Texture__H__

#include "Component.h"

class GameObject;

class C_Texture : public Component

{
public:
	C_Texture(GameObject* object);
	~C_Texture();
};

#endif __C__Texture__H__

