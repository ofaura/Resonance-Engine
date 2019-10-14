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
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilut.h"
#include "DevIL\include/IL\ilu.h"

#pragma comment( lib, "Devil/lib/x86/Release/DevIL.lib" )
#pragma comment( lib, "Devil/lib/x86/Release/ILU.lib" )
#pragma comment( lib, "Devil/lib/x86/Release/ILUT.lib" )

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module("Resource Manager", start_enabled){}
ModuleResourceManager::~ModuleResourceManager(){}

bool ModuleResourceManager::Init()
{
	LOG("Preparing resource loader");

	// DevIL version checking
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) {
		printf("DevIL version is different...exiting!\n");
		return 1;
	}

	// Initialize DevIL
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
	// Stream log messages to Assimp debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	// Load textures
	texture = GenerateTexture("Assets/Textures/Baker_house.png");

	// Loading FBX
	//LoadFBX("Assets/FBX/warrior.FBX"); 
	//LoadFBX("Assets/FBX/demon2.fbx", demon_tex); 
	//LoadFBX("Assets/FBX/BakerHouse.fbx");

	// Enable textures
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
	LOG("Freeing mesh loader");
	// Detach Assimp log stream
	aiDetachAllLogStreams();

	for (uint i = 0; i < MeshArray.size(); ++i)
		MeshArray[i].mesh.clear();

	MeshArray.clear();

	return true;
}

void ModuleResourceManager::LoadFilesFromPath(const char* path, uint tex) {

	Mesh fbx_mesh;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; ++i) {

			MeshData* m = new MeshData;

			// Copy vertices
			m->num_vertices = scene->mMeshes[i]->mNumVertices;
			m->vertices = new float[m->num_vertices * 3];
			memcpy(m->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * m->num_vertices * 3);
			LOG("New mesh with %d vertices", m->num_vertices);

			// Copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				m->num_indices = scene->mMeshes[i]->mNumFaces * 3;
				m->indices = new uint[m->num_indices]; // Assume each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
						LOG("WARNING, geometry face with != 3 indices!")
					else
						memcpy(&m->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}

			// Copy texture UVs
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				m->num_tex = scene->mMeshes[i]->mNumVertices;
				m->textures = new float[scene->mMeshes[i]->mNumVertices * 2];

				for (uint j = 0; j < scene->mMeshes[i]->mNumVertices; ++j)
				{
					m->textures[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					m->textures[(j * 2) + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}

			if (scene->mMeshes[i]->HasNormals()) {

			}

			// Assigning the VRAM
			glGenBuffers(1, (GLuint*)&m->id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, m->id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->num_vertices, m->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, (GLuint*)&m->id_index);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m->num_indices, m->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glGenBuffers(1, (GLuint*)&m->id_tex);
			glBindBuffer(GL_ARRAY_BUFFER, m->id_tex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * m->num_tex, m->textures, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			fbx_mesh.mesh.push_back(m);
		}

		if (tex == 0) fbx_mesh.texture = texture;
		else fbx_mesh.texture = tex;
		MeshArray.push_back(fbx_mesh);
		LOG("Loaded new model. Current FBX on scene: %d", App->rscr->MeshArray.size());

		aiReleaseImport(scene);
	}
	else LOG("Error loading FBX: %s", path);
}

void ModuleResourceManager::Draw(Mesh fbx_mesh) {

	for (uint i = 0; i < fbx_mesh.mesh.size(); ++i) {

		// Render the texture
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, fbx_mesh.texture);
		glActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, fbx_mesh.mesh[i]->id_tex);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		// Render the mesh
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, fbx_mesh.mesh[i]->id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbx_mesh.mesh[i]->id_index);
		glDrawElements(GL_TRIANGLES, fbx_mesh.mesh[i]->num_indices, GL_UNSIGNED_INT, NULL);

		// Clean all buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

uint ModuleResourceManager::GenerateTexture(const char* path)
{
	ILuint image;
	uint tex;
	ilGenImages(1, &image);
	ilBindImage(image);

	if (!ilLoadImage(path)) {
		ilDeleteImages(1, &image);
		LOG("The texture image could not be loaded")
			return 0;
	}
	else {
		tex = ilutGLBindTexImage();
		LOG("Created texture from: %s", path)

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

		return tex;
	}
}