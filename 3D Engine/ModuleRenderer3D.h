#ifndef __ModuleRenderer3D__
#define __ModuleRenderer3D__

#include "Module.h"
#include "glmath.h"
#include "Light.h"
#include "glew/include/GL/glew.h"

#define MAX_LIGHTS 8

class FBO;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void EnableDepthTest(bool active);
	void EnableCullFace(bool active);
	void EnableLighting(bool active);
	void EnableColorMaterial(bool active);
	void EnableTexture2D(bool active);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	FBO* fbo_tex;
};

#endif __ModuleRenderer3D__