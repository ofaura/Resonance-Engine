#include "GameObject.h"
#include "Application.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Texture.h"
#include "C_Camera.h"
#include "C_AudioSource.h"
#include "C_AudioListener.h"
#include "ModuleResourceManager.h"
#include "ModuleSceneIntro.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleSceneManager.h"

#ifdef _DEBUG
#pragma comment( lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib" )
#else
#pragma comment( lib, "MathGeoLib/libx86/Release/MathGeoLib.lib" )
#endif

#include "glmath.h"
#include "mmgr/mmgr.h"


GameObject::GameObject()
{
	MakeChild(App->scene_intro->root);
	UUID = (uint)App->GetRandom().Int();
	component_transform = (C_Transform*)AddComponent(COMPONENT_TYPE::TRANSFORM, true);
}

GameObject::GameObject(string name, GameObject* parent) : name(name), parent(parent)
{
	if (parent)
		MakeChild(parent);
	if (name != "root")
		UUID = (uint)App->GetRandom().Int();
	
	if (!App->scene->loadingScene)
	{
		component_transform = (C_Transform*)AddComponent(COMPONENT_TYPE::TRANSFORM, true);
	}
}

GameObject::GameObject(GameObject * parent, const char * name, const float3 & translation, const float3 & scale, const Quat & rotation)
{
	if (parent)
		MakeChild(parent);

	component_transform = (C_Transform*)AddComponent(COMPONENT_TYPE::TRANSFORM, true);
	component_transform->position = translation;
	component_transform->rotation = rotation;
	component_transform->scales = scale;
}

GameObject::~GameObject() {}

void GameObject::Update()
{
	
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i]->active) 
			components[i]->Update();
	}

	for (int i = 0; i < children.size(); ++i)
	{
		if (children[i]->enable)
		{
			children[i]->Update();
		}
	}

	Updatebbox();
	App->scene_intro->UpdateQuadtree();
}

void GameObject::PostUpdate()
{
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i]->active)
			components[i]->PostUpdate();
	}
	for (int i = 0; i < children.size(); ++i)
	{
		if (children[i]->enable)
		{
			children[i]->PostUpdate();
		}
	}


}

void GameObject::CleanUp()
{
	for (int i = 0; i < components.size(); i++)
		delete(components[i]);
}

void GameObject::EnableGO()
{
	if (!enable)
		enable = true;
}

void GameObject::DisableGO()
{
	if (enable)
		enable = false;
}

const int GameObject::GetId() const
{
	return UUID;
}

const char * GameObject::GetName() const
{
	return name.c_str();
}

bool & GameObject::GetActive()
{
	return enable;
}

void GameObject::SetName(const char * name)
{
	this->name = name;
}

Component* GameObject::AddComponent(COMPONENT_TYPE type, bool active)
{
	Component* component = nullptr;

	if (HasComponent(type)) return component;

	switch (type)
	{
		case COMPONENT_TYPE::TRANSFORM:
			component = new C_Transform(type, this, active);
			break;
		case COMPONENT_TYPE::MESH:
			component = new C_Mesh(type, this, active);
			break;
		case COMPONENT_TYPE::TEXTURE:
			component = new C_Texture(type, this, active);
			break;
		case COMPONENT_TYPE::CAMERA:
			component = new C_Camera(type, this, active);
			break;
		case COMPONENT_TYPE::AUDIO_SOURCE:
			component = new C_AudioSource(type, this, active);
			break;
		case COMPONENT_TYPE::AUDIO_LISTENER:
			component = new C_AudioListener(type, this, active);
			break;
	}

	if (component != nullptr)
		components.push_back(component);

	return component;
}

void GameObject::AddComponent(Component * component, COMPONENT_TYPE type)
{
	component->type = type;
	component->parent = this;
	components.push_back(component);
}

Component * GameObject::GetComponent(COMPONENT_TYPE type)
{
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
			return components[i];
	}

	return nullptr;
}

GameObject * GameObject::AddChildren(string name)
{
	GameObject* ret = new GameObject(name, this);
	mapChildren.insert({ ret->UUID, ret });

	return ret;
}

void GameObject::GetChildren(vector<GameObject*>* child)
{
	for (auto item = mapChildren.begin(); item != mapChildren.end(); ++item)
	{
		(*item).second->GetChildren(child);
	}
	child->push_back(this);
}

void GameObject::RemoveComponent(COMPONENT_TYPE type)
{
	for (vector<Component*>::iterator item = components.begin(); item != components.end(); ++item)
	{
		if ((*item)->GetType() == type)
		{
			components.erase(item);
			break;
		}
	}
}

bool GameObject::HasComponent(COMPONENT_TYPE type)
{
	for (vector<Component*>::iterator item = components.begin(); item != components.end(); ++item)
	{
		if ((*item)->GetType() == type)
			return true;
	}

	return false;
}

void GameObject::DrawInspector()
{
	for (int i = 0; i < components.size(); ++i)
		components[i]->DrawInspector();
}

void GameObject::MakeChild(GameObject * parent)
{
	this->parent = parent;
	parent->children.push_back(this);
	parentUUID = parent->GetId();
}

void GameObject::Load(const char * gameObject, const json & file)
{
	json tmp = file["Game Objects"][gameObject]["Name"];
	name = tmp.get<string>();
	UUID = file["Game Objects"][gameObject]["UUID"];
	parentUUID = file["Game Objects"][gameObject]["Parent UUID"];

	if (HasComponent(COMPONENT_TYPE::TRANSFORM) == false)
		component_transform = (C_Transform*)AddComponent(COMPONENT_TYPE::TRANSFORM);

	if (name == "Main Camera")
		AddComponent(COMPONENT_TYPE::CAMERA);
	
	else 
	{
		int numComponents = file["Game Objects"][gameObject]["Components"].size();

		if (numComponents > 1)
			AddComponent(COMPONENT_TYPE::MESH);
	
		if (numComponents > 2)
			AddComponent(COMPONENT_TYPE::TEXTURE);
	}

	for (uint i = 0; i < components.size(); ++i)
		components[i]->Load(gameObject, file);
}

void GameObject::Save(const char * gameObject, json & file)
{
	file["Game Objects"][gameObject]["Name"] = name;
	file["Game Objects"][gameObject]["UUID"] = UUID;
	file["Game Objects"][gameObject]["Parent UUID"] = parentUUID;

	for (uint i = 0; i < components.size(); ++i)
		components[i]->Save(gameObject, file);
}

void GameObject::Updatebbox() 
{
	obb = Localbbox;
	obb.Transform(mat2float4(this->component_transform->globalMatrix));
	
	Globalbbox.SetNegativeInfinity();
	Globalbbox.Enclose(obb);

	App->scene_intro->AABBInScene.push_back(&Globalbbox);
	App->scene_intro->OBBInScene.push_back(&obb);


}

void GameObject::UpdateChilds()
{
	if (enable)
	{
		for (int i = 0; i < children.size(); ++i)
		{
			children[i]->component_transform->UpdateMatrix();
			children[i]->UpdateChilds();
		}
	}
}

float4x4 GameObject::mat2float4(mat4x4 mat)
{
	float4x4 f_mat;
	f_mat.Set(mat.M);
	return f_mat.Transposed();
}

void GameObject::SetLocalAABB(AABB aabb)
{
	AABB compare;
	compare.SetNegativeInfinity();
	if (Localbbox.minPoint.Equals(compare.minPoint) && Localbbox.minPoint.Equals(compare.minPoint))
	{
		Localbbox = aabb;
		obb = Localbbox;
	}
	else
		Localbbox.Enclose(aabb);
}


