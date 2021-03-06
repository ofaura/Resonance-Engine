#ifndef __ModuleCamera3D__
#define __ModuleCamera3D__

#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include "C_Camera.h"



class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);

	float3 to_float3(vec3 vec);
	vec3 to_vec3(float3 vec);

private:

	bool MouseInsideWindow();

public:
	
	vec3 X, Y, Z, Position, Reference;
	float Sensitivity = 0.25f;
	float scrollSensitivity = 10.0f;
	float zoomSensitivity = 20.0f;
	float MidButtonSensitivity = 0.02;

	C_Camera* editorcamera = nullptr;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;

};

#endif __ModuleCamera3D__