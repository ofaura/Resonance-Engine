#ifndef __C__Transform__H__
#define __C__Transform__H__

#include "MathGeoLib/include/MathGeoLib.h"
#include "Component.h"
#include "glmath.h"

class GameObject;

class C_Transform : public Component
{
public:
	C_Transform(COMPONENT_TYPE type, GameObject* parent, bool active);
	~C_Transform();

	void DrawInspector();
	void Update() {};
	void UpdateMatrix();
	void Reset();

	void Load(const char* gameObject, const json &file);
	void Save(const char* gameObject, json &file);


	float4x4 mat2float4(mat4x4 mat);
	mat4x4 float2mat4(float4x4 flo);

	void SetLocalFromMatrix(mat4x4 matrix);
	mat4x4 GetLocalTransformMatrix();

	mat4x4 SetLocalMatrix(float3 pos, Quat rot, float3 scale);


public:

	mat4x4 globalMatrix;
	mat4x4 localMatrix;
	float4x4 f4x4_2_mat4x4;

	float3 position = float3(0, 0, 0);
	Quat rotation = Quat(0, 0, 0, 0);
	float3 scales = float3(1, 1, 1);

};

#endif __C__Transform__H__

