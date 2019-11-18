#ifndef __GameObject__H__
#define __GameObject__H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <vector>
#include <string>

class C_Mesh;
class C_Texture;
class C_Transform;
//class AABB;

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
	void RemoveComponent(COMPONENT_TYPE type);
	bool HasComponent(COMPONENT_TYPE type);

	const int GetId() const;
	const char* GetName() const;
	bool& GetActive();
	void SetName(const char* name);

	void DrawInspector();

private:

	uint id = 0;

public:

	bool enable = true;

	C_Mesh* component_mesh = nullptr;
	C_Texture* component_texture = nullptr;
	C_Transform* component_transform = nullptr;

	string name;
	GameObject* parent;
	vector<Component*> components;
	vector<GameObject*> children;

	AABB box;
};

#endif __GameObject__H__
