#ifndef __GameObject__H__
#define __GameObject__H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"

#include <vector>
#include <string>

class C_Mesh;
class C_Texture;
class C_Transform;
class C_Camera;

class GameObject
{
public:
	GameObject();
	GameObject(string name, GameObject* parent = nullptr);
	GameObject(GameObject* parent, const char* name = "Unnamed", const float3& translation = float3::zero, const float3& scale = float3::one, const Quat& rotation = Quat::identity);


	virtual ~GameObject();

	void Update();
	void PostUpdate();
	void CleanUp();

	void EnableGO();
	void DisableGO();

	Component* AddComponent(COMPONENT_TYPE type, bool active = true);
	void AddComponent(Component* component, COMPONENT_TYPE type);
	Component* GetComponent(COMPONENT_TYPE type);

	GameObject* AddChildren(string name);
	void GetChildren(vector<GameObject*>* child);

	void RemoveComponent(COMPONENT_TYPE type);
	bool HasComponent(COMPONENT_TYPE type);

	const int GetId() const;
	const char* GetName() const;
	bool& GetActive();
	void SetName(const char* name);
	void DrawInspector();
	void MakeChild(GameObject* parent);
	void Updatebbox();
	void UpdateChilds();

	float4x4 mat2float4(mat4x4 mat);

	void SetLocalAABB(AABB aabb);

	void RayHits(const LineSegment& segment, bool& hit, float& dist);

	void Load(const char* gameObject, const json &file);
	void Save(const char* gameObject, json &file);


public:

	uint UUID = 0;
	uint parentUUID = 0;

	bool toRender = false;
	bool enable = true;

	C_Transform* component_transform = nullptr;
	C_Camera* c_camera = nullptr;
	C_Mesh* c_mesh = nullptr;
	string name;
	GameObject* parent;
	vector<Component*> components;
	vector<GameObject*> children;

	map<uint, GameObject*> mapChildren;

	AABB Localbbox;
	AABB Globalbbox;
	OBB obb;

	bool HasToRender = true;

};

#endif __GameObject__H__
