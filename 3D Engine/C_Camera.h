#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "glmath.h"


#ifdef _DEBUG
#pragma comment (lib, "lib/MathGeoLib/libx86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "lib/MathGeoLib/libx86/Release/MathGeoLib.lib")
#endif


class C_Camera : public Component
{
public:

	C_Camera(COMPONENT_TYPE type, GameObject* gameobject );
	~C_Camera();

	void Update();
	void UpdateTransform(float4x4 global);

	//Getters
	void GetPlanes();
	float GetFOV();
	float GetAspectRatio();
	
	//Setters
	void SetNearPlane(float distance);
	void SetFarPlane(float distance);
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