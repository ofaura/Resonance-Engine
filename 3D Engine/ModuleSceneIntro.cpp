#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "Brofiler/Brofiler.h"
#include "EditorManager.h"
#include "GameObject.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module("SceneIntro", start_enabled) {}

ModuleSceneIntro::~ModuleSceneIntro() {}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG_CONSOLE("Loading Intro assets");
	LOG("Loading Intro assets");

	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	goSelected = &gameObjects.front();
	id_goSelected = gameObjects.size() - 1;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	objects_list.clear();

	return true;
}

Objects3D* ModuleSceneIntro::CreateObject3D(SHAPE_TYPE type, vec3 &position, vec3 &size, float color[4])
{
	Objects3D* ret = nullptr;

	switch (type)
	{
	case SHAPE_TYPE::TETRAHEDRON:
		ret = new Objects3D(SHAPE_TYPE::TETRAHEDRON, position, size, color);
		break;
	case SHAPE_TYPE::CUBE:
		ret = new Objects3D(SHAPE_TYPE::CUBE, position, size, color);
		break;
	case SHAPE_TYPE::OCTOHEDRON:
		ret = new Objects3D(SHAPE_TYPE::OCTOHEDRON, position, size, color);
		break;
	case SHAPE_TYPE::DODECAHEDRON:
		ret = new Objects3D(SHAPE_TYPE::DODECAHEDRON, position, size, color);
		break;
	case SHAPE_TYPE::ICOSAHEDRON:
		ret = new Objects3D(SHAPE_TYPE::ICOSAHEDRON, position, size, color);
		break;
	case SHAPE_TYPE::SPHERE:
		ret = new Objects3D(SHAPE_TYPE::SPHERE, position, size, color);
		break;
	case SHAPE_TYPE::CYLINDER:
		ret = new Objects3D(SHAPE_TYPE::CYLINDER, position, size, color);
		break;
	case SHAPE_TYPE::CONE:
		ret = new Objects3D(SHAPE_TYPE::CONE, position, size, color);
		break;
	case SHAPE_TYPE::PLANE:
		ret = new Objects3D(SHAPE_TYPE::PLANE, position, size, color);
		break;
	case SHAPE_TYPE::TORUS:
		ret = new Objects3D(SHAPE_TYPE::TORUS, position, size, color);
		break;
	}

	if (ret != nullptr)
		objects_list.push_back(ret);

	return ret;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::Beige)
	
	//grid
	Plane p(0, 1, 0, 0);
	
	p.axis = true;
	p.Render();

	for (list<Objects3D*>::iterator item = objects_list.begin(); item != objects_list.end(); ++item)
	{
		(*item)->Draw();
	}

	for (uint i = 0; i < gameObjects.size(); ++i) 
	{
		gameObjects[i].RenderGameObject();
		gameObjects[i].Update();
	}
	
	return UPDATE_CONTINUE;
}
