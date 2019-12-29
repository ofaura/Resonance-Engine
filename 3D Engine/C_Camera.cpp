#include "Globals.h"
#include "Application.h"
#include "C_Camera.h"
#include "C_Transform.h"
#include "ModuleSceneIntro.h"
#include "ModuleSceneManager.h"

#include "mmgr/mmgr.h"

C_Camera::C_Camera(COMPONENT_TYPE type,GameObject* gameobject, bool active) : Component(type, gameobject, active)
{

	name = "Camera";
	
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0, 0, 0);
	frustum.front = float3(0,0,1);
	frustum.up = float3(0,1,0);
	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 100.0f;
	SetFOV(90);
	GetPlanes();

}

C_Camera::~C_Camera(){}

void C_Camera::DrawInspector()
{

		ImGui::Checkbox("Draw Camera", &Draw_Cam);
		ImGui::DragInt("FOV", &fov, 1, 1, 200);
		if (ImGui::IsItemEdited())
		{
			SetFOV(fov);
		}
		ImGui::DragInt("Near Plane", &NearPlane, 1, 0, 50);
		if (ImGui::IsItemEdited())
		{
			SetPlanes(NearPlane, FarPlane);
		}
		ImGui::DragInt("Far Plane", &FarPlane, 1, 150, 300);
		if (ImGui::IsItemEdited()) 
		{ 
			SetPlanes(NearPlane, FarPlane);
		}
		
}

void C_Camera::Update()
{
	if (Draw_Cam)
	{
		Draw();
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


float * C_Camera::getViewMatrix()
{
	static float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

float * C_Camera::getProjectionMatrix()
{
	static float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

void C_Camera::SetPlanes(float neardistance, float fardistance)
{

	if (fardistance > 0 && fardistance > frustum.nearPlaneDistance)
		frustum.nearPlaneDistance = neardistance;

	if (neardistance > 0 && neardistance < frustum.farPlaneDistance)
		frustum.farPlaneDistance = fardistance;

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

float* C_Camera::ProjectionMatrix() 
{

	float4x4 m = frustum.ProjectionMatrix();
	m.Transpose();
	return (float*)m.v;

}

void C_Camera::Look(const float3 &pos) 
{

	float3 v = pos - frustum.pos;
	float3x3 matrix = float3x3::LookAt(frustum.front, v.Normalized(), frustum.up, float3(0,1,0));

	frustum.front = (matrix.MulDir(frustum.front).Normalized());
	frustum.up = (matrix.MulDir(frustum.up).Normalized());

	GetPlanes();

}

void C_Camera::Draw()
{
	float3 points[8];


	frustum.GetCornerPoints(points);

		glBegin(GL_LINES);
		glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
		glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
		glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
		glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
		glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
		glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
		glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
		glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));


		glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
		glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
		glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
		glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
		glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
		glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
		glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
		glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));

		glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
		glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
		glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
		glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
		glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
		glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
		glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
		glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));

		glEnd();
}

void C_Camera::Load(const char * gameObject, const json & file)
{
	active = file["Game Objects"][gameObject]["Components"]["Camera"]["Active"];
	fov = file["Game Objects"][gameObject]["Components"]["Camera"]["fov"];
	NearPlane = file["Game Objects"][gameObject]["Components"]["Camera"]["NearPlane"];
	FarPlane = file["Game Objects"][gameObject]["Components"]["Camera"]["FarPlane"];
}

void C_Camera::Save(const char * gameObject, json & file)
{
	file["Game Objects"][gameObject]["Components"]["Camera"]["Active"] = active;
	file["Game Objects"][gameObject]["Components"]["Camera"]["fov"] = fov;
	file["Game Objects"][gameObject]["Components"]["Camera"]["NearPlane"] = NearPlane;
	file["Game Objects"][gameObject]["Components"]["Camera"]["FarPlane"] = FarPlane;
}
