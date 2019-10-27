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
	if (ImGui::CollapsingHeader("Mesh"), ImGuiTreeNodeFlags_DefaultOpen)
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


}

void C_Mesh::DrawVerticesNormals()
{


}
