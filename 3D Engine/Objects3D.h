#ifndef __Objects3D__H__
#define __Objects3D__H__

#include "Globals.h"
#include "par_shapes.h"
#include "glmath.h"

#include "glew/include/GL/glew.h"

#include <vector>

using namespace std;

enum class SHAPE_TYPE 
{
	TETRAHEDRON,
	CUBE,
	OCTOHEDRON,
	DODECAHEDRON,
	ICOSAHEDRON,
	SPHERE,
	CYLINDER,
	CONE,
	PLANE,
	TORUS
};


class Objects3D
{
public:
	Objects3D::Objects3D(SHAPE_TYPE type, vec3 &position, vec3 &size);
	~Objects3D();

public:
	void Draw();

private:
	void TupleListToFlatList(par_shapes_mesh * mesh);

private:

	vector<float> triangle_vec;
	vector<float> normal_vec;
	
	float radius;
	int slices;
	int stacks;
	float center[3] = { 0, 0, 1 };
	float normal[3] = { 0, 0, 1 };
	float center2[3] = { 0, 0, 0 };
	float rotate[3] = { 1, 0, 0 };

	GLuint vertex_id;
	GLuint normal_id;

private:
	par_shapes_mesh* mesh = nullptr;
	par_shapes_mesh* disk1 = nullptr;
	par_shapes_mesh* disk2 = nullptr;
};

#endif __Objects3D__H__