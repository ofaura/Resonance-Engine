#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "ModuleCamera3D.h"
#include "Brofiler/Brofiler.h"
#include "glmath.h"

#include <gl/GL.h>
#include <gl/GLU.h>

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module("SceneIntro", start_enabled) {}

ModuleSceneIntro::~ModuleSceneIntro() {}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	App->log_list.push_back("Loading Intro assets");
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::Beige)
	
	//grid
	Plane p(0, 1, 0, 0);
	
	//draw test
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 10.f, 0.f);
	glEnd();
	glLineWidth(1.0f);

	p.axis = true;
	p.Render();


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision()
{
}

