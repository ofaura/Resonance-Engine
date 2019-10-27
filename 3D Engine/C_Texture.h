#ifndef __C__Texture__H__
#define __C__Texture__H__

#include "Component.h"

class GameObject;

class C_Texture : public Component

{
public:
	C_Texture(GameObject* object);
	~C_Texture();

	void DrawInspector();
	void Update() {};

public:
	uint texture = 0;
	uint original_texture = 0;
	string texture_path;

private:
	bool debug = false;
};

#endif __C__Texture__H__

