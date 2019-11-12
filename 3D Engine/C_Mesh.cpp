#include "C_Mesh.h"
#include "glmath.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "Inspector.h"
#include "EditorManager.h"

#include "glew/include/GL/glew.h"
#include "SDL\include\SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

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
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
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

void C_Mesh::DrawFaceNormals()
{
	for (uint i = 0; i < meshData.n_vertices; i += 3)
	{
		vec3 a = meshData.vertices[meshData.indices[i]];
		vec3 b = meshData.vertices[meshData.indices[i + 1]];
		vec3 c = meshData.vertices[meshData.indices[i + 2]];

		vec3 vec = CrossProduct((b - a), (c - a));
		vec3 newvec = normalize(vec);
		vec3 center = vec3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, (a.z + b.z + c.z) / 3);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(192, 192, 192);
		glVertex3f(center.x, center.y, center.z);
		glVertex3f((center.x + newvec.x * 0.5f),
			(center.y + newvec.y * 0.5f),
			(center.z + newvec.z * 0.5f));

		glColor3f(192, 192, 192);
		glEnd();
	}
}

void C_Mesh::DrawVerticesNormals()
{
	if (meshData.normals != nullptr) {

		for (uint i = 0; i < meshData.n_vertices; ++i) {
			vec3 point = meshData.vertices[i];
			vec3 vec = meshData.normals[i];

			glLineWidth(1.0f);
			glBegin(GL_LINES);
			glColor3f(192, 192, 192);
			glVertex3f(point.x, point.y, point.z);
			glVertex3f((point.x + vec.x * 0.5f),
				(point.y + vec.y * 0.5f),
				(point.z + vec.z * 0.5f));

			glColor3f(192, 192, 192);
			glEnd();
		}
	}
}

vec3 C_Mesh::CrossProduct(vec3 vect_A, vec3 vect_B)
{
	vec3 cross_P;

	cross_P.x = vect_A.y * vect_B.z - vect_A.z * vect_B.y;
	cross_P.y = vect_A.x * vect_B.z - vect_A.z * vect_B.x;
	cross_P.z = vect_A.x * vect_B.y - vect_A.y * vect_B.x;

	return cross_P;
}

vec3 C_Mesh::normalize(vec3 vect_A)
{
	double mag = sqrt(pow(vect_A.x, 2) + pow(vect_A.y, 2) + pow(vect_A.z, 2));
	vect_A.x = vect_A.x / mag;
	vect_A.y = vect_A.y / mag;
	vect_A.z = vect_A.z / mag;

	return vect_A;
}