#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "Brofiler/Brofiler.h"
#include "EditorManager.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "ModuleFileSystem.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Texture.h"

#include "mmgr/mmgr.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module("SceneIntro", start_enabled) 
{
}

ModuleSceneIntro::~ModuleSceneIntro() {}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG_CONSOLE("Loading Intro assets");
	LOG("Loading Intro assets");

	bool ret = true;

	root = new GameObject("root");

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//App->rscr->LoadFilesFBX("Assets/FBX/BakerHouse.fbx");
	/*int Size = gameObjects.size();
	if(Size != 0)
		goSelected = gameObjects.front();*/



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


GameObject * ModuleSceneIntro::AddGameObject(const char * name)
{
	GameObject* ret = new GameObject(name);

	ret->parent = root;
	root->children.push_back(ret);

	return ret;
}

void ModuleSceneIntro::SetParent(GameObject * newParent, GameObject * child)
{
	for (int i = 0; i < child->children.size(); ++i) 
	{
		GameObject* tmp = child;

		while (tmp->children.size() != 0)
		{
			if (child->children[i] == newParent)
				return;
		}
		
	}

	if (newParent->parent != nullptr)
	{
		for (int i = 0; i < child->parent->children.size(); ++i)
		{
			if (child->parent->children[i]->name == child->name)
			{
				child->parent->children.erase(child->parent->children.begin() + i);
				break;
			}
		}
	}

	/*for (int i = 0; i < newParent->children.size(); ++i)
	{
		if (child->parent->children[i]->name == child->name)
		{
			child->parent->children.erase(child->parent->children.begin() + i);
			break;
		}
	}*/



	child->parent = newParent;
	newParent->children.push_back(child);

}

string & ModuleSceneIntro::SetAvailableName(string name)
{
	uint num_version = 0;

	string final_name = name;
	for (int i = 0; i < root->children.size(); i++)
	{
		if (root->children[i]->name.c_str() == final_name)
		{
			num_version++;
			final_name = name + '(' + std::to_string(num_version) + ')';
			i = -1;
		}
	}
	name = final_name;

	return name;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::Beige)
	
	//grid
	Planes p(0, 1, 0, 0);
	
	p.axis = true;
	p.Render();

	for (list<Objects3D*>::iterator item = objects_list.begin(); item != objects_list.end(); ++item)
	{
		(*item)->Draw();
	}

	root->Update();

	root->children.size();
	if (ShowBoundingBoxes)
	{
		for (int i = 0; i < AABBInScene.size(); i++)
		{
			mesh->DrawBox(*AABBInScene[i],*OBBInScene[i]);
		}
	}
	
	return UPDATE_CONTINUE;
}
