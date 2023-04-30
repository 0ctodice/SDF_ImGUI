#pragma once
#include <glad/glad.h>
#include <iostream>

class FrameBuffer
{
public :
	FrameBuffer(GLuint width, GLuint height);
	~FrameBuffer();
	
	inline GLuint getFbo() const { return _fbo; }
	inline GLuint getTexture() const { return _texture; }
	inline GLuint getRenderBuffer() const { return _renderBuffer; }
	inline void getDimension(GLuint& w, GLuint& h) const { w = _width; h = _height; }
	inline void setClearColor(float r, float g, float b, float a) { _CCR = r; _CCG = g; _CCB = b; _CCA = a; }
	inline void getClearColor(float &r, float &g, float &b, float &a) { r = _CCR; g = _CCG; b = _CCB; a = _CCA; }
	void update(GLuint w, GLuint h);

private:
	GLuint _fbo = 0;
	GLuint _texture = 0;
	GLuint _renderBuffer = 0;
	GLuint _width, _height;
	float _CCR, _CCG, _CCB, _CCA;
	void generateFrameBuffer() {
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);
		glBindTexture(GL_TEXTURE_2D, 0);


		glGenRenderbuffers(1, &_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderBuffer);

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "Failed to create framebuffer, status : " << status << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};

