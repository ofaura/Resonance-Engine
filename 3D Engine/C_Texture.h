#ifndef __C__Texture__H__
#define __C__Texture__H__

#include "Component.h"

class GameObject;

class C_Texture : public Component

{
public:
	C_Texture(COMPONENT_TYPE type, GameObject* parent, bool active);
	~C_Texture();

	void DrawInspector();
	void Update() {};

	void Load(const char* gameObject, const json &file);
	void Save(const char* gameObject, json &file);

public:
	uint texture = 0;
	uint original_texture = 0;
	string texture_path;

	long width = 0;
	long height = 0;

private:
	bool debug = false;
};

#endif __C__Texture__H__

