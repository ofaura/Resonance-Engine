#include "C_Mesh.h"
#include "glmath.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "Inspector.h"
#include "EditorManager.h"

C_Mesh::C_Mesh(GameObject * object) : Component(COMPONENT_TYPE::MESH, object)
{
	name = "Mesh";
}

C_Mesh::~C_Mesh() {}

void C_Mesh::Update()
{
	if (drawFaceNormals)
		DrawFaceNormals();
	if (drawVerticesNormals)
		DrawVerticesNormals();
}

void C_Mesh::DrawInspector()
{
	if (ImGui::CollapsingHeader("Mesh")) 
	{
		ImGui::Checkbox("Enabled", &active);
		
		if (active)
		{
			ImGui::Text("Number of vertices: ");
			ImGui::SameLine();
			ImGui::Text("%d", meshData.n_vertices);

			ImGui::Text("Number of polygons: ");
			ImGui::SameLine();
			ImGui::Text("%d", meshData.n_indices / 3);

			ImGui::Separator();

			ImGui::Checkbox("Vertex normals", &drawVerticesNormals);
			ImGui::Checkbox("Face normals", &drawFaceNormals);
		}
		
	}
}

void C_Mesh::DrawFaceNormals()
{
	/*for (uint i = 0; i < meshData.n_indices; ++i)
	{
		vec3 a = vec3(meshData.vertices[i * 3], meshData.vertices[i * 3 + 1], meshData.vertices[i * 3 + 2]);
		vec3 b = vec3(meshData.vertices[i * 3 + 3], meshData.vertices[i * 3 + 4], meshData.vertices[i * 3 + 5]);
		vec3 c = vec3(meshData.vertices[i * 3 + 6], meshData.vertices[i * 3 + 7], meshData.vertices[i * 3 + 8]);

		vec3 vec = cross((b - a), (c - a));
		vec3 face_center = vec3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, (a.z + b.z + c.z) / 3);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(0, 255, 0);
		glVertex3f(face_center.x, face_center.y, face_center.z);
		glVertex3f((face_center.x + vec.x * 1.0f), (face_center.y + vec.y * 1.0f), (face_center.z + vec.z * 1.0f));

		glColor3f(255, 255, 255);
		glEnd();
	}*/


}

void C_Mesh::DrawVerticesNormals()
{
	//if (meshData.normals != nullptr)
	//{
	//	for (uint i = 0; i < meshData.n_vertices; ++i)
	//	{
	//		vec3 point = vec3(meshData.vertices[i * 3], meshData.vertices[i * 3 + 1], meshData.vertices[i * 3 + 2]);
	//		vec3 vec = vec3(meshData.normals[i * 3], meshData.normals[i * 3 + 1], meshData.normals[i * 3 + 2]);

	//		glLineWidth(1.0f);
	//		glBegin(GL_LINES);
	//		glColor3f(0, 255, 255);
	//		glVertex3f(point.x, point.y, point.z);
	//		glVertex3f((point.x + vec.x * 1.0f),
	//			(point.y + vec.y * 1.0f),
	//			(point.z + vec.z * 1.0f));

	//		glColor3f(255, 255, 255);
	//		glEnd();
	//	}
	//}
}
