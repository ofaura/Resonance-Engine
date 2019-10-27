#ifndef __ModuleSceneIntro__
#define __ModuleSceneIntro__

#include "Module.h"
#include "Primitive.h"
#include "Objects3D.h"
#include <vector>

#include "glew/include/GL/glew.h"

#include <gl/GL.h>
#include <gl/GLU.h>

class Objects3D;
class GameObject;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	
	Objects3D* CreateObject3D(SHAPE_TYPE type, vec3 &position, vec3 &size, float color[4]);

public:

	list<Objects3D*> objects_list;
	vector<GameObject*> gameObjects;
	GameObject* goSelected = nullptr;
	uint id_goSelected = 0;
};

	

#endif __ModuleSceneIntro__