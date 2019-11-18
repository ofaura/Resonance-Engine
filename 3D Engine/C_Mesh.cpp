#include "C_Mesh.h"
#include "C_Texture.h"
#include "Application.h"
#include "ModuleResourceManager.h"
#include "GameObject.h"
#include "mmgr/mmgr.h"


C_Mesh::C_Mesh(COMPONENT_TYPE type, GameObject * parent, bool active) : Component(type, parent, active) {}

C_Mesh::~C_Mesh() {}

void C_Mesh::Update()
{
	Render();

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
		float3 a = meshData.vertices[meshData.indices[i]];
		float3 b = meshData.vertices[meshData.indices[i + 1]];
		float3 c = meshData.vertices[meshData.indices[i + 2]];

		float3 vec = Cross((b - a), (c - a));
		vec.Normalize();
		float3 center = float3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, (a.z + b.z + c.z) / 3);

		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glColor3f(255, 255, 0);
		glVertex3f(center.x, center.y, center.z);
		glVertex3f((center.x + vec.x),
			(center.y + vec.y),
			(center.z + vec.z));

		glColor3f(255, 255, 255);
		glEnd();
	}
}

void C_Mesh::DrawVerticesNormals()
{
	if (meshData.normals != nullptr) {

		for (uint i = 0; i < meshData.n_vertices; ++i) {
			float3 point = meshData.vertices[i];
			float3 vec = meshData.normals[i];

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

float3 C_Mesh::CrossProduct(float3 vect_A, float3 vect_B)
{
	float3 cross_P;

	cross_P.x = vect_A.y * vect_B.z - vect_A.z * vect_B.y;
	cross_P.y = vect_A.x * vect_B.z - vect_A.z * vect_B.x;
	cross_P.z = vect_A.x * vect_B.y - vect_A.y * vect_B.x;

	return cross_P;
}

float3 C_Mesh::normalize(float3 vect_A)
{
	double mag = sqrt(pow(vect_A.x, 2) + pow(vect_A.y, 2) + pow(vect_A.z, 2));
	vect_A.x = vect_A.x / mag;
	vect_A.y = vect_A.y / mag;
	vect_A.z = vect_A.z / mag;

	return vect_A;
}

void C_Mesh::Render()
{
	C_Texture* texture = (C_Texture*)parent->GetComponent(COMPONENT_TYPE::TEXTURE);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (texture->active)
		glBindTexture(GL_TEXTURE_2D, texture->texture);

	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, meshData.id_texture);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	// Render the mesh
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, meshData.id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.id_index);
	glDrawElements(GL_TRIANGLES, meshData.n_indices * 3, GL_UNSIGNED_INT, NULL);

	// Clean all buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
