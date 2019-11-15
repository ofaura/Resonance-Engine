#include "C_Mesh.h"

//#include "glew/include/GL/glew.h"
//#include "SDL\include\SDL_opengl.h"
//
//#include "gl/GL.h"
//#include "gl/GLU.h""

//#include "MathGeoLib/include/MathGeoLib.h"
//#include "glmath.h"

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
	if(boundary_box)
		DrawBox();
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
		ImGui::Checkbox("Boundary Box", &boundary_box);
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

void C_Mesh::DrawBox() const
{
		float3 points[8];
		parent->box.GetCornerPoints(points);

		glBegin(GL_LINES);
		glColor3f(1, 0.84, 0);
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