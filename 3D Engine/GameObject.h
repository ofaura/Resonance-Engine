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

	void Start();
	void Update();
	void CleanUp();

	void EnableGO();
	void DisableGO();

	void AddComponent(COMPONENT_TYPE type);
	void RemoveComponent(COMPONENT_TYPE type);
	bool HasComponent(COMPONENT_TYPE type);


private:

	bool enable = false;
	uint id = 0;
	vector<Component*> components;

public:

	string name;
	GameObject* parent;
	vector<GameObject*> children;

};

#endif __GameObject__H__
