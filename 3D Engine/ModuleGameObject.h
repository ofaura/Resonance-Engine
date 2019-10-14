#ifndef __ModuleGameObject__H__
#define __ModuleGameObject__H__

#include "Module.h"

class ModuleGameObject : public Module
{
public:
	ModuleGameObject(bool start_enabled = true);
	~ModuleGameObject();

	bool Init(json file);
	
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	
	bool CleanUp();


};

#endif __ModuleGameObject__H__
