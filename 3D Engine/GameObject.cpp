#include "GameObject.h"
#include "C_Transform.h"

GameObject::GameObject(uint id) : id(id) {}

GameObject::~GameObject() {}

void GameObject::Start()
{
}

void GameObject::Draw()
{
}

void GameObject::CleanUp()
{
}

void GameObject::EnableGO()
{
}

void GameObject::DisableGO()
{
}

void GameObject::AddComponent(COMPONENT_TYPE type)
{
	if (HasComponent(type)) return;

	Component* component = nullptr;

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
