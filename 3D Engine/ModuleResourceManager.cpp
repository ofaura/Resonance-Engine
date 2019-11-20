#include "Application.h"
#include "Globals.h"
#include "ModuleResourceManager.h"
#include "ModuleSceneIntro.h"
#include "C_Texture.h"
#include "C_Mesh.h"
#include "ModuleFileSystem.h"

#include "glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "GameObject.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "DevIL/include/IL/ilut.h"

#include "MathGeoLib/include/MathGeoLib.h"

//#include "mmgr/mmgr.h"

#ifdef _DEBUG
#pragma comment( lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib" )
#else
#pragma comment( lib, "MathGeoLib/libx86/Release/MathGeoLib.lib" )
#endif

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment( lib, "DevIL/lib/x86/Release/DevIL.lib" )
#pragma comment( lib, "DevIL/lib/x86/Release/ILU.lib" )
#pragma comment( lib, "DevIL/lib/x86/Release/ILUT.lib" )


ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module("Resource Manager", start_enabled) {}
ModuleResourceManager::~ModuleResourceManager() {}

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

	loadedAll = true;

	GenerateCheckerTexture();

	glEnable(GL_TEXTURE_2D);

	return true;
}

bool ModuleResourceManager::CleanUp()
{
	aiDetachAllLogStreams();

	return true;
}

void ModuleResourceManager::LoadFilesFBX(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject* parent = new GameObject(GetNameFromPath(path));
		parent->MakeChild(App->scene_intro->root);

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			GameObject* fbxMesh = nullptr;

			if (scene->mNumMeshes > 1)
			{
				fbxMesh = new GameObject(scene->mMeshes[i]->mName.C_Str());
				fbxMesh->MakeChild(parent);
			}

			else
				fbxMesh = parent;

			// Copy the mesh						
			C_Mesh* mesh = (C_Mesh*)fbxMesh->AddComponent(COMPONENT_TYPE::MESH, fbxMesh);
			LoadMesh(mesh, scene->mMeshes[i]);

			// Copy materials
			aiMaterial* aux_mat = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
			uint totalTex = aux_mat->GetTextureCount(aiTextureType_DIFFUSE);
			aiString p;
			aux_mat->GetTexture(aiTextureType_DIFFUSE, 0, &p);

			string file = "Assets/Textures/";
			string name = GetNameFromPath(p.C_Str(), true);
			string full_path = file + name;

			C_Texture* texture = (C_Texture*)fbxMesh->AddComponent(COMPONENT_TYPE::TEXTURE, fbxMesh);
			GenerateTexture(full_path.c_str(), texture);


			App->scene_intro->gameObjects.push_back(fbxMesh);
			LOG("Loaded new model %s. GameObjects on scene: %d", fbxMesh->name.c_str(), App->scene_intro->gameObjects.size());
		}
		aiReleaseImport(scene);
	}
}

void ModuleResourceManager::ImportFile(const char * path)
{
	string finalPath;
	string extension;

	App->fileSystem->SplitFilePath(path, nullptr, &finalPath, &extension);

	if (CheckTextureExtension(extension.c_str()))
		finalPath = ASSETS_TEXTURE_FOLDER + finalPath;
	if (CheckMeshExtension(extension.c_str()))
		finalPath = ASSETS_MODEL_FOLDER + finalPath;

	if (App->fileSystem->CopyFromOutsideFS(path, finalPath.c_str()))
	{
		string written_file;
		if (CheckTextureExtension(extension.c_str()))
		{
			C_Texture* texture = (C_Texture*)App->scene_intro->goSelected->GetComponent(COMPONENT_TYPE::TEXTURE);
			if (!texture) 
				texture = (C_Texture*)App->scene_intro->goSelected->AddComponent(COMPONENT_TYPE::TEXTURE, App->scene_intro->goSelected);

			GenerateTexture(path, texture);
			texture = nullptr;
		}
		else if (CheckMeshExtension(extension.c_str())) 
		{
			C_Mesh* mesh = new C_Mesh(COMPONENT_TYPE::MESH, nullptr, true);
			LoadFilesFBX(path);
			App->scene_intro->goSelected = App->scene_intro->gameObjects.back();
		}
		else
			LOG("Importing of [%s] FAILED", finalPath.c_str())
	}
}

bool ModuleResourceManager::ImportTexture(const char * path, string & outputFile)
{
	bool ret = false;

	ILuint size;
	ILubyte* data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, NULL, 0);

	if (size > 0) 
	{
		data = new ILubyte[size];
		
		if (ilSaveL(IL_DDS, data, size) > 0)
			ret = App->fileSystem->SaveUnique(outputFile, data, size, LIBRARY_TEXTURES_FOLDER, GetNameFromPath(path).c_str(), "dds");
		RELEASE_ARRAY(data);

		LOG("Imported texture %s", outputFile);
	}

	if (!ret) 
		LOG("Cannot import texture from path %s", path);

	return ret;
}

bool ModuleResourceManager::ImportMesh(const char* path, C_Mesh* mesh)
{
	LOG("Importing mesh")
		bool ret = false;

	uint ranges[5] = 
	{
		mesh->meshData.n_indices,
		mesh->meshData.n_vertices,
		mesh->meshData.n_colors,
		mesh->meshData.n_normals,
		mesh->meshData.n_textures
	};

	uint size = sizeof(ranges) + sizeof(uint) * mesh->meshData.n_indices * 3 + sizeof(float3) * mesh->meshData.n_vertices
	+ sizeof(uint) * mesh->meshData.n_colors * 4 + sizeof(float3) * mesh->meshData.n_normals + sizeof(float) * mesh->meshData.n_textures * 2;

	if (size > 0)
	{
		char* data = new char[size];
		char* cursor = data;

		// Store ranges
		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);

		// Store indices
		cursor += bytes;
		bytes = sizeof(uint) * mesh->meshData.n_indices * 3;
		memcpy(cursor, mesh->meshData.indices, bytes);

		// Store vertices
		cursor += bytes;
		bytes = sizeof(float3) * mesh->meshData.n_vertices;
		memcpy(cursor, mesh->meshData.vertices, bytes);

		// Store colors
		cursor += bytes;
		bytes = sizeof(uint) * mesh->meshData.n_colors * 4;
		memcpy(cursor, mesh->meshData.colors, bytes);

		// Store normals
		cursor += bytes;
		bytes = sizeof(float3) * mesh->meshData.n_normals;
		memcpy(cursor, mesh->meshData.normals, bytes);

		// Store UVs
		cursor += bytes;
		bytes = sizeof(float) * mesh->meshData.n_textures * 2;
		memcpy(cursor, mesh->meshData.textures, bytes);

		ret = App->fileSystem->Save(path, data, size);
		RELEASE_ARRAY(data);
		cursor = nullptr;
	}

	if (!ret)
		LOG("Could not import mesh")

	return ret;
}

void ModuleResourceManager::LoadMesh(C_Mesh * mesh, aiMesh* currentMesh)
{
	Data data;

	// Copy vertices
	data.n_vertices = currentMesh->mNumVertices;
	data.vertices = new float3[data.n_vertices];
	memcpy(data.vertices, currentMesh->mVertices, sizeof(float3) * data.n_vertices);
	LOG("NEW MESH");
	LOG("Vertices: %d", data.n_vertices);

	// Copy faces
	if (currentMesh->HasFaces())
	{
		data.n_indices = currentMesh->mNumFaces;
		data.indices = new uint[data.n_indices * 3];
		for (uint j = 0; j < data.n_indices; ++j)
		{
			if (currentMesh->mFaces[j].mNumIndices != 3)
				LOG("WARNING, geometry face with != 3 indices!")
			else

				memcpy(&data.indices[j * 3], currentMesh->mFaces[j].mIndices, sizeof(uint) * 3);
		}
		LOG("Faces: %d", data.n_indices / 3);
	}

	// Copy normals
	if (currentMesh->HasNormals())
	{
		data.n_normals = currentMesh->mNumVertices;
		data.normals = new float3[data.n_normals];
		memcpy(data.normals, currentMesh->mNormals, sizeof(float3) * data.n_normals);
	}

	// Copy colors
	if (currentMesh->HasVertexColors(0))
	{
		data.n_colors = currentMesh->mNumVertices;
		data.colors = new uint[data.n_colors * 4];

		for (uint j = 0; j < data.n_colors; ++j)
		{
			data.colors[j * 4] = currentMesh->mColors[0][j].r;
			data.colors[j * 4 + 1] = currentMesh->mColors[0][j].g;
			data.colors[j * 4 + 2] = currentMesh->mColors[0][j].b;
			data.colors[j * 4 + 3] = currentMesh->mColors[0][j].a;
		}
	}

	// Copy texture UVs
	if (currentMesh->HasTextureCoords(0))
	{
		data.n_textures = currentMesh->mNumVertices;
		data.textures = new float[data.n_textures * 2];

		for (uint j = 0; j < data.n_textures; ++j)
		{
			data.textures[j * 2] = currentMesh->mTextureCoords[0][j].x;
			data.textures[j * 2 + 1] = currentMesh->mTextureCoords[0][j].y;
		}
	}

	// Generate AABB
	mesh->parent->Localbbox.SetNegativeInfinity();
	mesh->parent->Localbbox.Enclose((float3*)data.vertices, data.n_vertices);

	// Assigning the VRAM
	glGenBuffers(1, (GLuint*)&data.id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, data.id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * data.n_vertices, data.vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&data.id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 3 * data.n_indices, data.indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&data.id_texture);
	glBindBuffer(GL_ARRAY_BUFFER, data.id_texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * data.n_textures, data.textures, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mesh->meshData = data;
	mesh->name = mesh->parent->name;
	
	// We import the mesh to our library
	string path = LIBRARY_MESH_FOLDER + mesh->parent->name + ".mesh";;
	ImportMesh(path.c_str(), mesh);
}

void ModuleResourceManager::LoadMesh(const char * path, C_Mesh * mesh)
{
	char* buffer = nullptr;
	App->fileSystem->Load(path, &buffer);

	if (buffer) 
	{
		char* cursor = buffer;

		uint ranges[5];
		uint bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		mesh->meshData.n_indices = ranges[0] * 3;
		mesh->meshData.n_vertices = ranges[1];
		mesh->meshData.n_colors = ranges[2] * 4;
		mesh->meshData.n_normals = ranges[3];
		mesh->meshData.n_textures = ranges[4] * 2;

		// Load indices
		cursor += bytes;
		bytes = sizeof(uint) * mesh->meshData.n_indices;
		mesh->meshData.indices = new uint[mesh->meshData.n_indices];
		memcpy(mesh->meshData.indices, cursor, bytes);

		// Load vertices
		cursor += bytes;
		bytes = sizeof(float3) * mesh->meshData.n_vertices;
		mesh->meshData.vertices = new float3[mesh->meshData.n_vertices];
		memcpy(mesh->meshData.vertices, cursor, bytes);

		// Load colors
		cursor += bytes;
		bytes = sizeof(uint) * mesh->meshData.n_colors;
		mesh->meshData.colors = new uint[mesh->meshData.n_colors];
		memcpy(mesh->meshData.colors, cursor, bytes);

		// Load normals
		cursor += bytes;
		bytes = sizeof(float3) * mesh->meshData.n_normals;
		mesh->meshData.normals = new float3[mesh->meshData.n_normals];
		memcpy(mesh->meshData.normals, cursor, bytes);

		// Load Texture UVs
		cursor += bytes;
		bytes = sizeof(float) * mesh->meshData.n_textures;
		mesh->meshData.textures = new float[mesh->meshData.n_textures];
		memcpy(mesh->meshData.textures, cursor, bytes);

		// Assigning the VRAM
		glGenBuffers(1, (GLuint*)&mesh->meshData.id_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->meshData.id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * mesh->meshData.n_vertices, mesh->meshData.vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, (GLuint*)&mesh->meshData.id_index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->meshData.id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->meshData.n_indices, mesh->meshData.indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenBuffers(1, (GLuint*)&mesh->meshData.id_texture);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->meshData.id_texture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->meshData.n_textures, mesh->meshData.textures, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		RELEASE_ARRAY(buffer);
		cursor = nullptr;
	}
}

bool ModuleResourceManager::ValidTextureExtension(const string& extension)
{
	string ext;
	size_t i = extension.rfind('.', extension.length());
	if (i != string::npos) 
		ext = (char*)&(extension.substr(i + 1, extension.length() - i));
	
	return (ext == "dds" || ext == "png" || ext == "jpg" || ext == "DDS" || ext == "PNG" || ext == "JPG");
}

bool ModuleResourceManager::ValidMeshExtension(const string& extension)
{
	string ext;
	size_t i = extension.rfind('.', extension.length());
	if (i != string::npos)
		ext = (char*)&(extension.substr(i + 1, extension.length() - i));

	return (ext == "fbx" || ext == "FBX");
}

bool ModuleResourceManager::CheckTextureExtension(const char * extension)
{
	return (strcmp(extension, "dds") == 0 || strcmp(extension, "png") == 0 || strcmp(extension, "jpg") == 0);
}

bool ModuleResourceManager::CheckMeshExtension(const char * extension)
{
	return (strcmp(extension, "fbx") == 0 || strcmp(extension, "FBX") == 0);
}

void ModuleResourceManager::GenerateTexture(const char* path, C_Texture* texture)
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

	if (!ilLoadImage(path)) 
	{
		ilDeleteImages(1, &pic);
		return;
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

		if (texture)
		{
			texture->texture = aux_texture;
		}

		string name;
		ImportTexture(path, name);

		ilBindImage(0);
		ilDeleteImage(pic);

		texture->original_texture = aux_texture;
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

string ModuleResourceManager::GetNameFromPath(string path, bool withExtension)
{
	string name;

	App->fileSystem->NormalizePath(path);
	name = path.substr(path.find_last_of("//") + 1);

	if (withExtension)
		return name;
	
	else 
	{
		string::size_type const p(name.find_last_of('.'));
		string file_name = name.substr(0, p);

		return file_name;
	}
}



