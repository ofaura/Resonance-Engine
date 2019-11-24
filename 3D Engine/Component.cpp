#include "Component.h"
#include "Application.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

Component::Component(COMPONENT_TYPE type, GameObject* parent, bool active) : type(type) , parent(parent), active(active) 
{
	UUID = (uint)App->GetRandom().Int();
	if (parent) 
		parentUUID = parent->GetId();
}

Component::~Component() {}

void Component::Enable()
{
	if (!active)
		active = true;
}

void Component::Disable()
{
	if (active)
		active = false;
}

const COMPONENT_TYPE Component::GetType() const
{
	return type;
}

uint Component::GetUUID() const
{
	return UUID;
}

