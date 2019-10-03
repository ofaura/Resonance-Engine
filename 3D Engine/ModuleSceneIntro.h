#ifndef __ModuleSceneIntro__
#define __ModuleSceneIntro__

#include "Module.h"
#include "Primitive.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
};


#endif __ModuleSceneIntro__