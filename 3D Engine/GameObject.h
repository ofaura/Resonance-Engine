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
	void Draw();
	void CleanUp();

	void EnableGO();
	void DisableGO();

	void AddComponent(COMPONENT_TYPE type);
	void RemoveComponent(COMPONENT_TYPE type);
	bool HasComponent(COMPONENT_TYPE type);

private:

private:

	bool enable = false;
	uint id = 0;
	
	vector<Component*> components;
};

#endif __GameObject__H__
