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
#include "DevIL/include/IL/ilu.h"
#include "GameObject.h"
#include "C_Texture.h"
#include "C_Mesh.h"
#include "ModuleSceneIntro.h"

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
	
	texture = GenerateTexture("Assets/Baker_house.png");
	LoadFilesFromPath("Assets/BakerHouse.fbx");
	loadedAll = true;

	GenerateCheckerTexture();

	glEnable(GL_TEXTURE_2D);

	return true;
}

update_status ModuleResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResourceManager::CleanUp()
{
	aiDetachAllLogStreams();

	return true;
}

void ModuleResourceManager::LoadFilesFromPath(const char* path, uint tex) {

	

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; ++i) {

			string n = path;
			string directory;
			string name;
			uint j;

			if (loadedAll)
				j = n.rfind('\\', n.length());
			else
				j = n.rfind('//', n.length());

			if (j != string::npos)
				directory = (n.substr(j + 1, n.length() - j));

			const uint last_slash_idx = directory.rfind('.');

			if (string::npos != last_slash_idx)
				name = directory.substr(0, last_slash_idx);

			uint count = 0;
			for (uint k = 0; k < App->scene_intro->gameObjects.size(); ++k)
			{
				if (App->scene_intro->gameObjects.at(k)->name == name)
				{
					count++;
					name.append(" (");
					name.append(std::to_string(count));
					name.append(")");
				}
			}

			GameObject *Loadmesh = new GameObject(name);

			Data data;

			// Copy vertices
			data.n_vertices = scene->mMeshes[i]->mNumVertices;
			data.vertices = new float[data.n_vertices * 3];
 			memcpy(data.vertices, scene->mMeshes[i]->mVertices, sizeof(float) * data.n_vertices * 3);

			glGenBuffers(1, (GLuint*)&data.id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, data.id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * data.n_vertices, data.vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				data.n_indices = scene->mMeshes[i]->mNumFaces * 3;
				data.indices = new uint[data.n_indices];

				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
						memcpy(&data.indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}

			glGenBuffers(1, (GLuint*)&data.id_index);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.id_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * data.n_indices, data.indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// Copy texture UVs
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				data.n_textures = scene->mMeshes[i]->mNumVertices;
				data.textures = new float[scene->mMeshes[i]->mNumVertices * 2];

				for (uint j = 0; j < scene->mMeshes[i]->mNumVertices; ++j)
				{
					data.textures[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					data.textures[(j * 2) + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}

			glGenBuffers(1, (GLuint*)&data.id_texture);
			glBindBuffer(GL_ARRAY_BUFFER, data.id_texture);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * data.n_textures, data.textures, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);		

			Loadmesh->component_mesh->meshData = data;
			
			if (tex == 0)
				Loadmesh->component_texture->texture = texture;
			else 
				Loadmesh->component_texture->texture = tex;
			
			App->scene_intro->gameObjects.push_back(Loadmesh);
		}
		aiReleaseImport(scene);
	}
	else LOG("Error loading FBX: %s", path);
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

void ModuleResourceManager::GenerateCheckerTexture()
{
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	for (int i = 0; i < CHECKERS_HEIGHT; i++)
	{
		for (int j = 0; j < CHECKERS_WIDTH; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checker_texture);
	glBindTexture(GL_TEXTURE_2D, checker_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}