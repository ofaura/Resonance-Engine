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
	void DrawSolidSphere(GLfloat X, GLfloat Y, GLfloat Z, float radius, uint rings, uint sectors);

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
};


#endif __ModuleSceneIntro__