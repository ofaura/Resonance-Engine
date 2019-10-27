#include "Component.h"

Component::Component(COMPONENT_TYPE type, GameObject* parent, bool active) : type(type) , parent(parent), active(active) {}

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

