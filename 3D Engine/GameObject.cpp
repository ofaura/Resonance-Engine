#include "GameObject.h"
#include "Application.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Texture.h"


GameObject::GameObject(uint id) : id(id) {}

GameObject::~GameObject() {}

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

void GameObject::AddComponent(COMPONENT_TYPE type)
{
	Component* component = nullptr;

	if (HasComponent(type)) return;


	switch (type)
	{
		case COMPONENT_TYPE::TRANSFORM:
			component = new C_Transform(this);
			break;
		case COMPONENT_TYPE::MESH:
			component = new C_Transform(this);
			break;
	}

	if (component != nullptr)
		components.push_back(component);
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

void GameObject::RenderGameObject()
{



}
