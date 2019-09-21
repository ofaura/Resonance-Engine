#ifndef __ModuleH__
#define __ModuleH__

#include "Globals.h"
#include <string>

using namespace std;

class Application;

class Module
{
private :
	bool enabled;

public:
	string name;

	Module(const char* name, bool start_enabled = true) : name(name)
	{
		enabled = start_enabled;
	}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision()
	{}
};

#endif __ModuleH__