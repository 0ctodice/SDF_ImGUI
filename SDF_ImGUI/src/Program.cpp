#include "Program.h"

Program::~Program(){ glDeleteProgram(_shaderProgram); }

void Program::createProgram(const char* VS, const char* FS) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VS, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FS, NULL);
	glCompileShader(fragmentShader);

	_shaderProgram = glCreateProgram();

	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);

	glLinkProgram(_shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}