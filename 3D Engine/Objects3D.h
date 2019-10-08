#ifndef __Objects3D__H__
#define __Objects3D__H__

#include "Globals.h"
#include "par_shapes.h"
#include "glmath.h"

#include "glew/include/GL/glew.h"

#include <vector>

using namespace std;

class Objects3D
{
public:
	Objects3D::Objects3D(vec3 position, vec3 size);
	~Objects3D();

public:
	void Draw();

private:
	void TupleListToFlatList(par_shapes_mesh * mesh);

private:
	par_shapes_mesh* mesh = nullptr;

	vector<float> triangle_vec;
	vector<float> normal_vec;
	
	GLuint vertex_id;
	GLuint normal_id;

};

#endif __Objects3D__H__