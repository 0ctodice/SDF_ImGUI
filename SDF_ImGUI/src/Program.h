#pragma once
#include <stdio.h>
#include <glad/glad.h>

class Program
{
private :
	GLuint _shaderProgram = NULL;

public :
	~Program();
	void createProgram(const char* VS, const char* FS);
	inline void useProgram() const { glUseProgram(_shaderProgram); }
	inline GLuint getGLid() const { return _shaderProgram; }
};

