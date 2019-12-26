#ifndef __ModuleSceneIntro__
#define __ModuleSceneIntro__

#include "Module.h"
#include "Primitive.h"
#include "Objects3D.h"
#include <vector>

#include "glew/include/GL/glew.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"
#include "C_Mesh.h"
#include "Quadtree.h"

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
	update_status PostUpdate(float dt);
	bool CleanUp();
	
	Objects3D* CreateObject3D(SHAPE_TYPE type, vec3 &position, vec3 &size, float color[4]);
	
	GameObject*  CreateGameObject(const char* name);
	void SetParent(GameObject* newParent, GameObject* Child);

	string& SetAvailableName(string name);

	void UpdateQuadtree();

public:

	list<Objects3D*> objects_list;
	vector<GameObject*> gameObjects;

	GameObject* goSelected = nullptr;
	GameObject* root = nullptr;
	uint id_goSelected = 0;

	vector<AABB*> AABBInScene;
	vector<OBB*> OBBInScene;

	C_Mesh* mesh;
	GameObject* MainCamera;
	//Quadtree* objectTree;

	bool ShowBoundingBoxes = false;

	bool ShowQuadtree = true;

public:

	GameObject* music = nullptr;
	GameObject* car = nullptr;
	GameObject* helicopter = nullptr;
};

	

#endif __ModuleSceneIntro__