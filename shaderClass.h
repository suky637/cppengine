#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <cerrno>
#include "filemanager.h"


class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	GLuint getLoc(const char* location);
	void Activate();
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};