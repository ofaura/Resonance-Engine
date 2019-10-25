#ifndef __GameObject__H__
#define __GameObject__H__

#include "Globals.h"
#include "Component.h"

#include <vector>

using namespace std;

class GameObject
{
public:
	GameObject(uint id);
	virtual ~GameObject();

	void CleanUp();

	void EnableGO();
	void DisableGO();

	Component* AddComponent(COMPONENT_TYPE type);
	void RemoveComponent(COMPONENT_TYPE type);
	bool HasComponent(COMPONENT_TYPE type);


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
