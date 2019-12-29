#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"


class C_Camera : public Component
{
public:

	C_Camera(COMPONENT_TYPE type, GameObject* gameobject, bool active);
	~C_Camera();

	void DrawInspector();
	void Update();
	void UpdateTransform(float4x4 global);

	//Getters
	void GetPlanes() { frustum.GetPlanes(planes); };
	float GetFOV() { return frustum.verticalFov * RADTODEG; };
	float GetAspectRatio() { return frustum.AspectRatio(); };

	float* getViewMatrix();
	float* getProjectionMatrix();
	
	//Setters
	void SetPlanes(float neardistance, float fardistance);
	void SetFOV(float fov, float width = 16.0f, float height = 9.0f);
	void SetPos(float3 position);
	void SetPos(vec3 position);
	void SetAspectRatio(float ar);

	//Projection and ViewMatrix
	float* ViewMatrix();
	float* ProjectionMatrix();
	
	void Look(const float3 & Spot);

	void Draw();

	void Load(const char* gameObject, const json &file);
	void Save(const char* gameObject, json &file);

public:
	
	Frustum frustum;
	Plane planes[6];
	int fov = 60;
	int NearPlane = 1;
	int FarPlane = 200;

private:

	bool Draw_Cam = true;

};

#endif