#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"


class C_Camera : public Component
{
public:

	C_Camera(COMPONENT_TYPE type, GameObject* gameobject );
	~C_Camera();

	void DrawInspector();
	void UpdateTransform(float4x4 global);

	//Getters
	void GetPlanes() { frustum.GetPlanes(planes); };
	float GetFOV() { return frustum.verticalFov * RADTODEG; };
	float GetAspectRatio() { return frustum.AspectRatio(); };
	
	//Setters
	void SetPlanes(float neardistance, float fardistance);
	void SetFOV(float fov, float width = 16.0f, float height = 9.0f);
	void SetPos(float3 position);
	void SetPos(vec3 position);
	void SetAspectRatio(float ar);

	//Projection and ViewMatrix
	float* ViewMatrix();
	mat4x4 ViewMatrix4x4();
	float* ProjectionMatrix();
	mat4x4 ProjectionMatrix4x4();
	
	void Look(const float3 & Spot);

public:
	
	Frustum frustum;
	Plane planes[6];
	int fov = 60;
};

#endif