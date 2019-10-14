#include "ModuleResourceManager.h"
#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilut.h"
#include "DevIL\include/IL\ilu.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment( lib, "DevIL/lib/x86/Release/DevIL.lib" )
#pragma comment( lib, "DevIL/lib/x86/Release/ILU.lib" )
#pragma comment( lib, "DevIL/lib/x86/Release/ILUT.lib" )

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module("Resource Manager", start_enabled){}
ModuleResourceManager::~ModuleResourceManager(){}

bool ModuleResourceManager::Init()
{
	ilInit();
	iluInit();
	ilEnable(IL_CONV_PAL);
	ilutEnable(ILUT_OPENGL_CONV);
	ilutRenderer(ILUT_OPENGL);
	ilutInit();

	return true;
}

bool ModuleResourceManager::Start()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	glEnable(GL_TEXTURE_2D);

	return true;
}

update_status ModuleResourceManager::Update(float dt)
{
	for (uint i = 0; i < MeshArray.size(); ++i)
		Draw(MeshArray[i]);

	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	aiDetachAllLogStreams();
	
	for (uint i = 0; i < MeshArray.size(); ++i) MeshArray[i].mesh.clear();
	MeshArray.clear();

	return true;
}

void ModuleResourceManager::LoadFilesFromPath(const char* path, uint tex) {

	Mesh Loadmesh;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i) {

			Data* data = new Data;

			// Copy vertices
			data->num_vertices = scene->mMeshes[i]->mNumVertices;
			data->vertices = new float[data->num_vertices * 3];
			memcpy(data->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * data->num_vertices * 3);

			glGenBuffers(1, (GLuint*)&data->id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, data->id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * data->num_vertices, data->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				data->num_indices = scene->mMeshes[i]->mNumFaces * 3;
				data->indices = new uint[data->num_indices];

				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
						memcpy(&data->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}

			glGenBuffers(1, (GLuint*)&data->id_index);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->id_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * data->num_indices, data->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// Copy texture UVs
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				data->num_texture = scene->mMeshes[i]->mNumVertices;
				data->textures = new float[scene->mMeshes[i]->mNumVertices * 2];

				for (uint j = 0; j < scene->mMeshes[i]->mNumVertices; ++j)
				{
					data->textures[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					data->textures[(j * 2) + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}

			glGenBuffers(1, (GLuint*)&data->id_texture);
			glBindBuffer(GL_ARRAY_BUFFER, data->id_texture);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * data->num_texture, data->textures, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			Loadmesh.mesh.push_back(data);
		}

		if (tex == 0) 
			Loadmesh.texture = texture;
		else 
			Loadmesh.texture = tex;
		
		MeshArray.push_back(Loadmesh);

		aiReleaseImport(scene);
	}
	else LOG("Error loading FBX: %s", path);
}

void ModuleResourceManager::Draw(Mesh fbx_mesh) {

	for (uint i = 0; i < fbx_mesh.mesh.size(); ++i) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);// Draw texture
		glBindTexture(GL_TEXTURE_2D, fbx_mesh.texture);
		glActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, fbx_mesh.mesh[i]->id_texture);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		glEnableClientState(GL_VERTEX_ARRAY);// Draw mesh
		glBindBuffer(GL_ARRAY_BUFFER, fbx_mesh.mesh[i]->id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx_mesh.mesh[i]->id_index);
		glDrawElements(GL_TRIANGLES, fbx_mesh.mesh[i]->num_indices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);//Clean up
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

uint ModuleResourceManager::GenerateTexture(const char* path)
{

	ilInit();
	iluInit();

	ilEnable(IL_CONV_PAL);
	ilutEnable(ILUT_OPENGL_CONV);
	ilutRenderer(ILUT_OPENGL);

	ilutInit();

	ILuint pic;
	uint aux_texture;

	ilGenImages(1, &pic);
	ilBindImage(pic);

	if (!ilLoadImage(path)) {
		ilDeleteImages(1, &pic);
			return 0;
	}
	else 
	{	
		aux_texture = ilutGLBindTexImage();
		
		long width;
		long height;
		long bit_depth;
		long format;

		ILubyte *texdata = 0;

		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);
		bit_depth = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
		format = ilGetInteger(IL_IMAGE_FORMAT);
		texdata = ilGetData();

		glGenTextures(1, &aux_texture);
		
		glBindTexture(GL_TEXTURE_2D, aux_texture);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		gluBuild2DMipmaps(GL_TEXTURE_2D, bit_depth, width, height, format, GL_UNSIGNED_BYTE, texdata);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		ilBindImage(0);
		ilDeleteImage(pic);

		return aux_texture;
	}
}