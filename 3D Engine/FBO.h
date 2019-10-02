#ifndef __FBO__H__
#define __FBO__H__

#include "glew/include/GL/glew.h"
#include "Globals.h"

class FBO
{
public:
	
	FBO();
	~FBO();

	void CreateFBO(uint width, uint height);
	void BindFBO();
	void UnbindFBO();
	void DeleteFBO();
	GLuint GetTexture();

private:

	GLuint fbo;
	GLuint texColorBuffer;
	GLuint rbo;
	int width;
	int height;
};

#endif __FBO__H__