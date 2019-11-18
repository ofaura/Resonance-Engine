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
	GameObject();
	GameObject(string name, GameObject* parent = nullptr);

	virtual ~GameObject();

	void Update();
	void CleanUp();

	void EnableGO();
	void DisableGO();

	Component* AddComponent(COMPONENT_TYPE type, bool active = true);
	Component* GetComponent(COMPONENT_TYPE type);

	void RemoveComponent(COMPONENT_TYPE type);
	bool HasComponent(COMPONENT_TYPE type);

	const int GetId() const;
	const char* GetName() const;
	bool& GetActive();
	void SetName(const char* name);

	void DrawInspector();
	void RenderGameObject() const;

	void MakeChild(GameObject* parent);

private:

	uint uid = 0;

public:

	bool enable = true;

	C_Transform* component_transform = nullptr;

	string name;
	GameObject* parent;
	vector<Component*> components;
	vector<GameObject*> children;
};

#endif __GameObject__H__
