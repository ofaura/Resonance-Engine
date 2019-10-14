#ifndef __Component__H__
#define __Component__H__

enum class COMPONENT_TYPE
{
	TRANSFORM,
};

class Component
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

	virtual void Start() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	void Enable();
	void Disable();

	const COMPONENT_TYPE GetType();

private:

	COMPONENT_TYPE type;
	bool active = false;
};

#endif __Component__H__
