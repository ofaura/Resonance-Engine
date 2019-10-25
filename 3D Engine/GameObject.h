#ifndef __GameObject__H__
#define __GameObject__H__

#include "Globals.h"
#include "Component.h"

#include <vector>
#include <string>

class C_Mesh;
class C_Texture;
class C_Transform;

class GameObject
{
public:
	GameObject(uint id);
	virtual ~GameObject();

	void CleanUp();

	void EnableGO();
	void DisableGO();

	void AddComponent(COMPONENT_TYPE type);
	void RemoveComponent(COMPONENT_TYPE type);
	bool HasComponent(COMPONENT_TYPE type);

	void RenderGameObject();

private:

	bool enable = false;
	uint id = 0;
	

public:

	string name;
	GameObject* parent;
	vector<GameObject*> children;
	vector<Component*> components;
};

#endif __GameObject__H__
