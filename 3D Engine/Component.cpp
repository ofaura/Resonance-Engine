#include "Component.h"

Component::Component(COMPONENT_TYPE type) : type(type) {}

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

const COMPONENT_TYPE Component::GetType()
{
	return type;
}
