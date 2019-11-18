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

#include "mmgr/mmgr.h"


GameObject::GameObject()
{
}

GameObject::GameObject(string name, GameObject* parent) : name(name), parent(parent)
{
	//id = App->GetRandom().Int();
	component_transform = (C_Transform*)AddComponent(COMPONENT_TYPE::TRANSFORM, true);
	component_mesh = (C_Mesh*)AddComponent(COMPONENT_TYPE::MESH);
	component_texture = (C_Texture*)AddComponent(COMPONENT_TYPE::TEXTURE);
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
	return id;
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
			component = new C_Mesh(this);
			break;
		case COMPONENT_TYPE::TEXTURE:
			component = new C_Texture(this);
			break;
	}

	if (component != nullptr)
		components.push_back(component);

	return component;
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
