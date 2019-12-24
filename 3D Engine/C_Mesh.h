#ifndef __C__Mesh__H__
#define __C__Mesh__H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "GameObject.h"

class GameObject;
class C_Texture;

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

	uint n_normals = 0;
	float3* normals = nullptr;
	
	uint n_colors = 0;
	uint* colors = nullptr;

	void CleanUp();
};

class C_Mesh : public Component

{
public:
	C_Mesh(COMPONENT_TYPE type, GameObject* parent, bool active);
	~C_Mesh();

	void Update() override;
	void PostUpdate() override;
	void CleanUp() override;
	void DrawInspector();
	void DrawFaceNormals();
	void DrawVerticesNormals();
	void DrawBox(AABB& bbox, OBB& obb);

	float3 CrossProduct(float3 vect_A, float3 vect_B);
	float3 normalize(float3 vect_A);

	void Load(const char* gameObject, const json &file);
	void Save(const char* gameObject, json &file);

	
	void SetTexture(C_Texture* texture);
	void Render();

	bool Intersect(Frustum frustum, AABB aabb);

private:
	bool drawFaceNormals = false;
	bool drawVerticesNormals = false;
	bool boundary_box = false;

	C_Camera* auxcam = nullptr;


public:
	Data meshData;
	string name;

	
	float3 pos = { 0,0,0 };
	AABB bbox;

	C_Texture* texture = nullptr;
};

#endif __C__Mesh__H__

