#include "C_Mesh.h"

C_Mesh::C_Mesh(GameObject * object) : Component(COMPONENT_TYPE::MESH, object)
{
	name = "Mesh";
}

C_Mesh::~C_Mesh() {}
