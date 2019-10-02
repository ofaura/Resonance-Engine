#include "FBO.h"
#include "Application.h"
#include "ModuleWindow.h"

FBO::FBO() 
{
	fbo = 0;
	texColorBuffer = 0;
	rbo = 0;
	width = 0;
	height = 0;
}

FBO::~FBO() {}

void FBO::CreateFBO(uint _width, uint _height)
{
	width = _width;
	height = _height;

	// Creating a framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// generate texture
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// depth texture
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::BindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FBO::UnbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::DeleteFBO()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &texColorBuffer);
	glDeleteTextures(1, &rbo);
}

uint FBO::GetTexture()
{
	return fbo;
}
