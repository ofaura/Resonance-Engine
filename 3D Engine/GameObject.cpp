#include "GameObject.h"
#include "C_Transform.h"
#include "Application.h"

GameObject::GameObject(uint id) : id(id) , name("GameObject")
{
	components.push_back(AddComponent(COMPONENT_TYPE::TRANSFORM));
}

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

Component* GameObject::AddComponent(COMPONENT_TYPE type)
{
	Component* component = nullptr;

	if (HasComponent(type)) return component;


	switch (type)
	{
		case COMPONENT_TYPE::TRANSFORM:
			component = new C_Transform();
			break;
	}

	if (component != nullptr)
	{
		component->Start();
		components.push_back(component);
	}

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
