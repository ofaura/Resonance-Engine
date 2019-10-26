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
	GameObject(string name, GameObject* parent = nullptr);

	virtual ~GameObject();

	void Update();
	void CleanUp();

	void EnableGO();
	void DisableGO();

	Component* AddComponent(COMPONENT_TYPE type, bool active = true);
	void RemoveComponent(COMPONENT_TYPE type);
	bool HasComponent(COMPONENT_TYPE type);

	const int GetId() const;
	const char* GetName() const;
	const bool GetSelected() const;
	void SetSelected(const bool& selected);

	void DrawInspector();
	void RenderGameObject() const;

private:

	bool enable = false;
	uint id = 0;
	bool is_selected;

public:

	C_Mesh* component_mesh = nullptr;
	C_Texture* component_texture = nullptr;
	C_Transform* component_transform = nullptr;

	string name;
	GameObject* parent;
	vector<Component*> components;
	vector<GameObject*> children;

};

#endif __GameObject__H__
