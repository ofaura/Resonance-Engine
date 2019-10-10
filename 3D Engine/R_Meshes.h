#ifndef __R_MESHES_H__
#define __R_MESHES_H__

#include "Globals.h"
#include "Math.h"
#include "glmath.h"
#include "Resources.h"

struct aiMesh;

class Meshes : public Resources
{
public:

	Meshes();
	virtual ~Meshes();


	void Importer(aiMesh* mesh);

public:

	vec3* Vertices = nullptr;
	uint VerticesID = 0;
	uint VerticesSize = 0;
	uint* Indices = nullptr;
	uint IndicesID = 0;
	uint IndicesSize = 0;
	vec3* Normals = nullptr;
	uint NormalsSize = 0;
	vec2* TexCoords = nullptr;
	uint  TexID = 0;
	uint TexCoordsSize = 0;
	unsigned char* Colours = nullptr;
	uint ColoursSize = 0;

};

#endif