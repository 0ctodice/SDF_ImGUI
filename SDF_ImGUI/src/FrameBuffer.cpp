#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(GLuint width, GLuint height) : _width{ width }, _height{ height }, _CCR{ 1 }, _CCG{ 1 }, _CCB{ 1 }, _CCA{ 1 }{
	generateFrameBuffer();
}

void FrameBuffer::update(GLuint w, GLuint h) {
	if (w != _width && h != _height) {
		_width = w;
		_height = h;
		generateFrameBuffer();
	}
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &_fbo);
	_fbo = 0;
	_texture = 0;
	_renderBuffer = 0;
}
