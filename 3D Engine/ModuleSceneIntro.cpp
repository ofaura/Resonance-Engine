#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "ModuleCamera3D.h"
#include "Brofiler/Brofiler.h"
#include "glmath.h"
#include "EditorManager.h"

#include <vector>

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

	DrawCubeWithQuads(0, 0, 4, 3, green);
	DrawCubeWithTriangles(-4, 0, 4, 3, blue);
	DrawSolidSphere(4, 0, 4, 1.5, 12, 24);

	p.axis = true;
	p.Render();


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawCubeWithQuads(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength, GLfloat color[])
{
	GLfloat halfSideLength = edgeLength * 0.5;

	GLfloat vertices[] =
	{
		//front face
		centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // top left
		centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // top right
		centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // bottom right
		centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // bottom left

		//back face
		centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top left
		centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
		centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
		centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom left

		//left face
		centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
		centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
		centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
		centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

		//right face
		centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // top left
		centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // top right
		centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // bottom right
		centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // bottom left

		//top face
		centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top left
		centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top right
		centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // bottom right
		centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // bottom left

		//bottom face
		centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // top left
		centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // top right
		centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
		centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glColorPointer(3, GL_FLOAT, 0, color);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 24);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleSceneIntro::DrawCubeWithTriangles(GLfloat X, GLfloat Y, GLfloat Z, GLfloat edgeLength, GLfloat color[])
{
	GLfloat l = edgeLength / 2;

	GLubyte indices[] =
	{
		0, 1, 2,   2, 3, 0,     // front
		4, 5, 6,   6, 7, 4,     // right
		8, 9,10,  10,11, 8,     // top
		12,13,14,  14,15,12,    // left
		16,17,18,  18,19,16,    // bottom
		20,21,22,  22,23,20		// back
	};

	GLfloat vertices[] =
	{
		l, l, l,  -l, l, l,  -l,-l, l,   l,-l, l,   // v0,v1,v2,v3 (front)
		l, l, l,   l,-l, l,   l,-l,-l,   l, l,-l,   // v0,v3,v4,v5 (right)
		l, l, l,   l, l,-l,  -l, l,-l,  -l, l, l,   // v0,v5,v6,v1 (top)
		-l, l, l,  -l, l,-l,  -l,-l,-l,  -l,-l, l,  // v1,v6,v7,v2 (left)
		-l,-l,-l,   l,-l,-l,   l,-l, l,  -l,-l, l,  // v7,v4,v3,v2 (bottom)
		l,-l,-l,  -l,-l,-l,  -l, l,-l,   l, l,-l	// v4,v7,v6,v5 (back)
	};

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glColorPointer(3, GL_FLOAT, 0, color);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTranslatef(X, Y, Z);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleSceneIntro::DrawSolidSphere(GLfloat X, GLfloat Y, GLfloat Z, float radius, uint rings, uint sectors)
{
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLfloat> texcoords;
	vector<GLushort> indices;

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texcoords.resize(rings * sectors * 2);
	
	vector<GLfloat>::iterator v = vertices.begin();
	vector<GLfloat>::iterator n = normals.begin();
	vector<GLfloat>::iterator t = texcoords.begin();
	
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) 
	{
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*t++ = s * S;
		*t++ = r * R;

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

		*n++ = x;
		*n++ = y;
		*n++ = z;
	}
	
	indices.resize(rings * sectors * 4);
	vector<GLushort>::iterator i = indices.begin();
	
	for (r = 0; r < rings - 1; r++)
	{
		for (s = 0; s < sectors - 1; s++) 
		{

			*i++ = (r + 1) * sectors + s;
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = r * sectors + (s + 1);
			*i++ = r * sectors + s;
		}
	}
		
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
	glTranslatef(X, Y, Z);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
