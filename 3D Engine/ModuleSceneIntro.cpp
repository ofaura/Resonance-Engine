#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "Brofiler/Brofiler.h"
#include "EditorManager.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "ModuleInput.h"
#include "ModuleFileSystem.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Texture.h"
#include "C_AudioSource.h"
#include "Game\Assets\Sounds\Wwise_IDs.h"
#include "ModuleSceneManager.h"
#include "SpacePartitioning.h"
#include "Game.h"
#include "ModuleAudio.h"

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
	MainCamera = new GameObject("Main Camera", root);
	MainCamera->AddComponent(COMPONENT_TYPE::CAMERA, true);
	C_Camera* cam = (C_Camera*)MainCamera->GetComponent(COMPONENT_TYPE::CAMERA);
	cam->SetPlanes(1,300);
	MainCamera->AddComponent(COMPONENT_TYPE::AUDIO_LISTENER, true);

	MainCamera->component_transform->position.z = 100;
	MainCamera->component_transform->rotation.y = 180;
	MainCamera->component_transform->UpdateMatrix();

	App->camera->LookAt(vec3(0, 0, 0));

	music = CreateGameObject("music");
	music->AddComponent(COMPONENT_TYPE::AUDIO_SOURCE);
	C_AudioSource* musicSource = (C_AudioSource*)music->GetComponent(COMPONENT_TYPE::AUDIO_SOURCE);
	musicSource->SetID(AK::EVENTS::BACKGROUNDMUSIC);
	musicSource->wwiseGO->PlayEvent(AK::EVENTS::BACKGROUNDMUSIC);
	musicSource->isPlaying = true;

	helicopter = App->rscr->FileReceived("Assets/FBX/Mi28.fbx");
	helicopter->name = "helicopter (dynamic)";
	helicopter->AddComponent(COMPONENT_TYPE::AUDIO_SOURCE);
	C_AudioSource* helicopterSource = (C_AudioSource*)helicopter->GetComponent(COMPONENT_TYPE::AUDIO_SOURCE);
	helicopter->component_transform->rotation.x = -90;
	helicopter->component_transform->rotation.z = 180;
	helicopter->component_transform->position.y = 5;
	helicopter->component_transform->UpdateMatrix();
	helicopterSource->SetID(AK::EVENTS::HELICOPTER);
	App->audio->Tests(helicopterSource->wwiseGO->GetID());

	car = App->rscr->FileReceived("Assets/FBX/FordFiestaR2.fbx");
	car->name = "car";
	car->AddComponent(COMPONENT_TYPE::AUDIO_SOURCE);
	C_AudioSource* carSource = (C_AudioSource*)car->GetComponent(COMPONENT_TYPE::AUDIO_SOURCE);
	car->component_transform->rotation.x = -172;
	car->component_transform->position.y = 3.5;
	car->component_transform->position.x = -2.2;

	car->component_transform->scales = float3(0.01, 0.01, 0.01);
	car->component_transform->UpdateMatrix();
	carSource->SetID(AK::EVENTS::FRANCESCO);


	//initialize The space partition from root tree
	AABB rootAABB;
	rootAABB.SetNegativeInfinity();
	rootTree = new SpacePartitioning(rootAABB, 0);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	objects_list.clear();

	root->CleanUp();

	rootTree->Clear();
	RELEASE(rootTree);

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


GameObject * ModuleSceneIntro::CreateGameObject(const char * name)
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
	
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		helicopter->component_transform->position.z -= 1;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		helicopter->component_transform->position.z += 1;
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		helicopter->component_transform->position.x -= 1;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		helicopter->component_transform->position.x += 1;

	helicopter->component_transform->UpdateMatrix();

	//C_AudioSource* musicSource = (C_AudioSource*)music->GetComponent(COMPONENT_TYPE::AUDIO_SOURCE);
	//musicSource->wwiseGO->SetAuxSends();
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


	if (rootTree->TreeBox.IsFinite()) {
		std::vector<GameObject*> objects;

		rootTree->Intersect(objects, App->camera->editorcamera->frustum);


		for (auto it : objects) {
			
			if (it->GetComponent(COMPONENT_TYPE::MESH) != nullptr) {
				it->HasToRender = ContainsBox(it->Globalbbox);
			}
		}
	}


	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->editor->game->isMouseOnScene())
		MousePicking();

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Scene PostUpdate", Profiler::Color::Beige)

	root->PostUpdate();

	if (TreeNeedsUpdate) {
		SetQuadTree();
		TreeNeedsUpdate = false;
	}

	DebugDrawLine(line);

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::GetAllGO(GameObject * go)
{
	if (go != root)
		allGO.push_back(go);
	else if (go == root)
		allGO.clear();
	for (auto it : go->children) {
		GetAllGO(it);
	}
}

void ModuleSceneIntro::AddGOToTree(GameObject * go)
{
	if (go == nullptr)
		return;

	if (go->GetComponent(COMPONENT_TYPE::MESH) != nullptr) {
		rootTree->Insert(go);
	}
	for (auto it : go->children)
		AddGOToTree(it);
}

void ModuleSceneIntro::SetTreeSize(GameObject* go) {
	if (go == nullptr)
		return;
	if (go->GetComponent(COMPONENT_TYPE::MESH) != nullptr)
	{
		rootTree->TreeBox.Enclose(go->Globalbbox);
		for (auto it : go->children)
		{
			SetTreeSize(it);
		}
	}
}

void ModuleSceneIntro::SetQuadTree()
{
	allGO.clear();
	rootTree->Clear();
	GetAllGO(root);

	for (auto it : allGO) {
		SetTreeSize(it);
	}
	for (auto j : allGO) {
		//AddGOToTree(j);
	}
}

bool ModuleSceneIntro::ContainsBox(const AABB& refBox) const
{
	float3 vCorner[8];
	int iTotalIn = 0;
	refBox.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array

	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes

			if (App->camera->editorcamera->frustum.GetPlane(p).IsOnPositiveSide(vCorner[i]))
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		if (iInCount == 0)
			return false;
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}

	return true;
}

void ModuleSceneIntro::MousePicking()
{
	ImVec2 normalized = App->editor->game->GetMouse();

	if (normalized.x > -1 && normalized.x < 1) {
		if (normalized.y > -1 && normalized.y < 1) {


			LineSegment ray;

				ray = App->camera->editorcamera->frustum.UnProjectLineSegment(normalized.x, normalized.y);

			GetAllGO(root);
			float distance = 9 * 10 ^ 10;
			GameObject* closest = nullptr;
			//drawRay = true;
			line = ray;
			for (auto it : allGO)
			{
				bool hit;
				float dist;
				it->RayHits(ray, hit, dist);

				if (hit)
				{
					if (dist < distance)
					{
						distance = dist;
						closest = it;
					}
				}
			}

			if (closest != nullptr)
			{
				/*DeselectAll();
				ShowGameObjectInspector(closest);*/
				goSelected = closest;
			}
		}
	}
}

void ModuleSceneIntro::DebugDrawLine(const LineSegment line, const float4x4 & transform, Color color, float lineWidth) {
	glColor3f(color.r, color.g, color.b);
	glLineWidth(lineWidth);

	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	glBegin(GL_LINES);

	glVertex3fv((GLfloat*)&line.a);
	glVertex3fv((GLfloat*)&line.b);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	glEnd();

	glPopMatrix();
}