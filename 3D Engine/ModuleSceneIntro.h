#ifndef __ModuleSceneIntro__
#define __ModuleSceneIntro__

#include "Module.h"
#include "Primitive.h"

#include "glew/include/GL/glew.h"

#include <gl/GL.h>
#include <gl/GLU.h>

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:

	void DrawCubeWithQuads(GLfloat X, GLfloat Y, GLfloat Z, GLfloat edgeLength, GLfloat color[]);
	void DrawCubeWithTriangles(GLfloat X, GLfloat Y, GLfloat Z, GLfloat edgeLength, GLfloat color[]);
	void DrawCubeWithVertexArrays(GLfloat X, GLfloat Y, GLfloat Z, GLfloat edgeLength, GLfloat color[]);
	void DrawSolidSphere(GLfloat X, GLfloat Y, GLfloat Z, float radius, uint rings = 12, uint sectors = 24);

private:

	GLfloat white[72] = { 1, 1, 1,   1, 1, 1,   1, 1, 1,   1, 1, 1,
				   1, 1, 1,   1, 1, 1,   1, 1, 1,   1, 1, 1,   
				   1, 1, 1,   1, 1, 1,   1, 1, 1,   1, 1, 1,   
				   1, 1, 1,   1, 1, 1,   1, 1, 1,   1, 1, 1,   
				   1, 1, 1,   1, 1, 1,   1, 1, 1,   1, 1, 1,   
				   1, 1, 1,   1, 1, 1,   1, 1, 1,   1, 1, 1 };
	
	GLfloat red[72] = { 1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   
				   1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,  
				   1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   
				   1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,  
				   1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   
				   1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0 }; 

	GLfloat blue[72] = { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   
				   0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,
				   0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,
				   0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,
				   0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,
				   0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1 };
	
	GLfloat green[72] = { 0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,
				   0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,
				   0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,
				   0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,
				   0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,
				   0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0 };

	GLfloat yellow[72] = { 1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0,
				   1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0,
				   1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0,
				   1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0,
				   1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0,
				   1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0 };

	GLfloat colors[72] =
	{
		 1, 1, 1,   1, 1, 0,   1, 0, 0,   1, 0, 1,  // v0,v1,v2,v3 (front)
		 1, 1, 1,   1, 0, 1,   0, 0, 1,   0, 1, 1,  // v0,v3,v4,v5 (right)
		 1, 1, 1,   0, 1, 1,   0, 1, 0,   1, 1, 0,  // v0,v5,v6,v1 (top)
		 1, 1, 0,   0, 1, 0,   0, 0, 0,   1, 0, 0,  // v1,v6,v7,v2 (left)
		 0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0,  // v7,v4,v3,v2 (bottom)
		 0, 0, 1,   0, 0, 0,   0, 1, 0,   0, 1, 1   // v4,v7,v6,v5 (back)
	};

private:
	GLuint vboId = 0;
	GLuint iboId = 0;
};


#endif __ModuleSceneIntro__