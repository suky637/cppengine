#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

struct SimpleVertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VBO
{
public:
	GLuint ID;
	VBO(std::vector<Vertex>& vertices);
	VBO(std::vector<SimpleVertex>& vertices);

	void Bind();
	void UnBind();
	void Delete();
};