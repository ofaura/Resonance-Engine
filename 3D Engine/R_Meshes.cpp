#include "R_Meshes.h"
#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "glmath.h"

Meshes::Meshes() : Resources(Resources::RType::mesh){}
Meshes::~Meshes(){}

void Meshes::Importer(aiMesh* mesh)
{
	this->VerticesSize = mesh->mNumVertices;
	this->Vertices = new vec3[mesh->mNumVertices];

	for (unsigned j = 0; j < mesh->mNumVertices; ++j){ this->Vertices[j] = *((vec3*)&mesh->mVertices[j]); }

	glGenBuffers(1, (GLuint*)&this->VerticesID);
	glBindBuffer(GL_ARRAY_BUFFER, this->VerticesID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * this->VerticesSize, this->Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	if (mesh->HasNormals())
	{
		NormalsSize = mesh->mNumVertices;
		Normals = new vec3[NormalsSize];
		memcpy(Normals, mesh->mNormals, sizeof(vec3)*NormalsSize);
	}

	if (mesh->HasTextureCoords(0))
	{
		TexCoordsSize = mesh->mNumVertices;
		TexCoords = new vec2[TexCoordsSize];
		for (unsigned i = 0; i < TexCoordsSize; ++i) { TexCoords[i] = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);}
	}

	glGenTextures(1, (GLuint*)&this->TexID);
	glBindTexture(GL_TEXTURE_2D, this->TexID);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,sizeof(vec3)*TexCoordsSize,0,GL_RGBA, GL_UNSIGNED_BYTE,); // send vertices to VRAM
	glBindTexture(GL_TEXTURE_2D, 0);

	if (mesh->HasVertexColors(0))
	{
		ColoursSize = VerticesSize;
		Colours = new unsigned char[ColoursSize * 4];
		memcpy(Colours, mesh->mColors, sizeof(unsigned char)*ColoursSize * 4);
	}

	this->IndicesSize = mesh->mNumFaces * 3;
	this->Indices = new uint[this->IndicesSize];

	for (unsigned j = 0; j < mesh->mNumFaces; ++j)
	{
		const aiFace& face = mesh->mFaces[j];
		assert(face.mNumIndices == 3);
		this->Indices[j * 3] = face.mIndices[0];
		this->Indices[j * 3 + 1] = face.mIndices[1];
		this->Indices[j * 3 + 2] = face.mIndices[2];
	}

	glGenBuffers(1, (GLuint*)&this->IndicesID); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * this->IndicesSize, this->Indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}