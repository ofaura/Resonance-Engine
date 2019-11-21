#include "Globals.h"
#include "Application.h"
#include "C_Camera.h"

C_Camera::C_Camera(COMPONENT_TYPE type,GameObject* gameobject) : Component(type, gameobject)
{

	name = "Camera";
	
	
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0, 0, 0);
	frustum.front = float3(0,0,1);
	frustum.up = float3(0,1,0);
	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 2000.0f;
	
	if (gameobject == nullptr) 
	{
		frustum.verticalFov = 1.0f;
		frustum.horizontalFov = 1.0f;
	}
	else 
	{
		SetFOV(60);
	}
	
	GetPlanes();

}

C_Camera::~C_Camera(){}

void C_Camera::DrawInspector()
{

		ImGui::DragInt("FOV", &fov, 1, 1, 200);

		if (ImGui::IsItemEdited()) 
		{ 
			SetFOV(fov);
		}

}

void C_Camera::UpdateTransform(float4x4 global) 
{
	
	frustum.front = global.WorldZ();
	frustum.up = global.WorldY();

	float3 position, scale;
	Quat quat;

	global.Decompose(position, quat, scale);

	SetPos(position);
	GetPlanes();

}


void C_Camera::SetPlanes(float neardistance, float fardistance) 
{

	if (fardistance > 0 && fardistance > frustum.nearPlaneDistance)
		frustum.nearPlaneDistance = fardistance;

	if (neardistance > 0 && neardistance < frustum.farPlaneDistance)
		frustum.farPlaneDistance = neardistance;

	GetPlanes();

}

void C_Camera::SetFOV(float fov, float width, float height) 
{

	float ar = height / width;
	frustum.verticalFov = fov * DEGTORAD;
	frustum.horizontalFov = 2.f * Atan(Tan(frustum.verticalFov*0.5f) / ar);
	GetPlanes();

}

void C_Camera::SetPos(float3 position) 
{

	frustum.pos = position;
	GetPlanes();

}

void C_Camera::SetPos(vec3 position) 
{

	frustum.pos = float3(position.x, position.y, position.z);
	GetPlanes();

}

void C_Camera::SetAspectRatio(float ar) 
{

	frustum.verticalFov = 2.f * Atan(Tan(frustum.horizontalFov*0.5f) / ar);
	GetPlanes();

}

float* C_Camera::ViewMatrix() 
{

	float4x4 m = frustum.ViewMatrix();
	m.Transpose();
	return (float*)m.v;

}

mat4x4 C_Camera::ViewMatrix4x4() 
{

	float4x4 m = frustum.ViewMatrix();
	m.Transpose();
	mat4x4 m4 = { m.At(0,0), m.At(0,1), m.At(0,2), m.At(0,3),
				 m.At(1,0), m.At(1,1), m.At(1,2), m.At(1,3),
				 m.At(2,0), m.At(2,1), m.At(2,2), m.At(2,3),
				 m.At(3,0), m.At(3,1), m.At(3,2), m.At(3,3) };
	return m4;

}

float* C_Camera::ProjectionMatrix() 
{

	float4x4 m = frustum.ProjectionMatrix();
	m.Transpose();
	return (float*)m.v;

}

mat4x4 C_Camera::ProjectionMatrix4x4() 
{

	float4x4 m = frustum.ProjectionMatrix();
	m.Transpose();
	mat4x4 m4 = { m.At(0,0), m.At(0,1), m.At(0,2), m.At(0,3),
				 m.At(1,0), m.At(1,1), m.At(1,2), m.At(1,3),
				 m.At(2,0), m.At(2,1), m.At(2,2), m.At(2,3),
				 m.At(3,0), m.At(3,1), m.At(3,2), m.At(3,3) };
	return m4;

}

void C_Camera::Look(const float3 &pos) 
{

	float3 v = pos - frustum.pos;
	float3x3 matrix = float3x3::LookAt(frustum.front, v.Normalized(), frustum.up, float3(0,1,0));

	frustum.front = (matrix.MulDir(frustum.front).Normalized());
	frustum.up = (matrix.MulDir(frustum.up).Normalized());

	GetPlanes();

}
