#include "ModuleResourceManager.h"
#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "R_Meshes.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilut.h"
#include "DevIL\include/IL\ilu.h"

#pragma comment( lib, "Devil/lib/x86/Release/DevIL.lib" )
#pragma comment( lib, "Devil/lib/x86/Release/ILU.lib" )
#pragma comment( lib, "Devil/lib/x86/Release/ILUT.lib" )

void MyAssimpCallback(const char * msg, char * userData)
{
	LOG("[Assimp]: %s", msg);
}

ModuleResourceManager::ModuleResourceManager(bool start_enabled) : Module("Resource Manager", start_enabled){}
ModuleResourceManager::~ModuleResourceManager(){}

bool ModuleResourceManager::Init(json file)
{
	struct aiLogStream stream;
	stream.callback = MyAssimpCallback;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	// DevIL version checking
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
		printf("DevIL version is different");

	return true;
}

bool ModuleResourceManager::Start()
{
	// Enable textures
	glEnable(GL_TEXTURE_2D);

	// Load textures	
	CheckersTexture("Assets/Baker_house.png");

	return true;
}

bool ModuleResourceManager::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	for (uint i = 0; i < meshes.size(); ++i)
	{
		glDeleteBuffers(1, (GLuint*)&meshes[i]->VerticesID);
		glDeleteBuffers(1, (GLuint*)&meshes[i]->IndicesID);
		RELEASE_ARRAY(meshes[i]->Vertices);
		RELEASE_ARRAY(meshes[i]->Indices);
		delete meshes[i];
	}

	return true;
}

void ModuleResourceManager::Draw()
{

	for (uint i = 0; i < meshes.size(); ++i)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->VerticesID);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->IndicesID);
		glDrawElements(GL_TRIANGLES, meshes[i]->IndicesSize, GL_UNSIGNED_INT, NULL);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glDisableClientState(GL_VERTEX_ARRAY);

		if (meshes[i]->TexCoords)
		{
			glEnable(GL_TEXTURE_2D);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->TexID);
			glDrawArrays(GL_TEXTURE_2D, 0, meshes[i]->TexCoordsSize);
			glDisable(GL_TEXTURE_2D);

		}

		if (meshes[i]->Normals)
		{
			glBegin(GL_LINES);
			glLineWidth(1.0f);
			uint Normal_length = 1;
			glColor4f(0.0f, 0.5f, 0.5f, 1.0f);

			for (uint j = 0; j < meshes[i]->VerticesSize; ++j)
			{
				glVertex3f(meshes[i]->Vertices[j].x, meshes[i]->Vertices[j].y, meshes[i]->Vertices[j].z);
				glVertex3f(meshes[i]->Vertices[j].x + meshes[i]->Normals[j].x*Normal_length, meshes[i]->Vertices[j].y + meshes[i]->Normals[j].y*Normal_length, meshes[i]->Vertices[j].z + meshes[i]->Normals[j].z*Normal_length);
			}

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glEnd();

		}
	}
}

bool ModuleResourceManager::LoadFileFromPath(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			Meshes* mesh_aux = new Meshes;
			meshes.push_back(mesh_aux);
			aiMesh* mesh = scene->mMeshes[i];
			mesh_aux->Importer(mesh);
		}
		aiReleaseImport(scene);
	}
	else { LOG("|[error]: Error loading scene %s", path); }
	
	return true;
}

void ModuleResourceManager::CheckersTexture(const char* path)
{
	ILuint image; 
	ilGenImages(1, &image); 
	ilBindImage(image);

	if (!ilLoadImage(path))
		ilDeleteImages(1, &image);
	else {

		tex = ilutGLBindTexImage();

		long h, v, bpp, f;
		ILubyte *texdata = 0;

		h = ilGetInteger(IL_IMAGE_WIDTH);
		v = ilGetInteger(IL_IMAGE_HEIGHT);
		bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
		f = ilGetInteger(IL_IMAGE_FORMAT);
		texdata = ilGetData();

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, h, v, f, GL_UNSIGNED_BYTE, texdata);
		glBindTexture(GL_TEXTURE_2D, 0);

		ilBindImage(0);
		ilDeleteImage(image);
	}
}