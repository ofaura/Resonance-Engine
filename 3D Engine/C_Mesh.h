#ifndef __C__Mesh__H__
#define __C__Mesh__H__

#include "Component.h"
#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

struct Data {

	uint id_index = 0;
	uint n_indices = 0;
	uint* indices = nullptr;

	uint id_vertex = 0;
	uint n_vertices = 0;
	float3* vertices = nullptr;

	uint id_texture = 0;
	uint n_textures = 0;
	float* textures = nullptr;

	vec3* normals = nullptr;

	uint n_colors = 0;
	uint* colors = nullptr;
};

class C_Mesh : public Component

{
public:
	C_Mesh(GameObject* object);
	~C_Mesh();

	void Update() override;
	void DrawInspector();
	void DrawFaceNormals();
	void DrawVerticesNormals();

private:
	bool drawFaceNormals = false;
	bool drawVerticesNormals = false;

public:
	Data meshData;

};

#endif __C__Mesh__H__

