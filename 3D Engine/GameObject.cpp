#include "GameObject.h"
#include "Application.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Texture.h"
#include "ModuleResourceManager.h"
#include "ModuleSceneIntro.h"
#include "MathGeoLib/include/MathGeoLib.h"

#ifdef _DEBUG
#pragma comment( lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib" )
#else
#pragma comment( lib, "MathGeoLib/libx86/Release/MathGeoLib.lib" )
#endif

#include "glmath.h"
#include "mmgr/mmgr.h"


GameObject::GameObject()
{
	component_transform = (C_Transform*)AddComponent(COMPONENT_TYPE::TRANSFORM, true);
	uid = (uint)App->GetRandom().Int();
}

GameObject::GameObject(string name, GameObject* parent) : name(name), parent(parent)
{
	uid = (uint)App->GetRandom().Int();
	component_transform = (C_Transform*)AddComponent(COMPONENT_TYPE::TRANSFORM, true);
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


}

void GameObject::CleanUp()
{
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
	return uid;
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
	}

	if (component != nullptr)
		components.push_back(component);

	return component;
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
}

