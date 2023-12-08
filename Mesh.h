#pragma once

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "camera.h"
#include "Texture.h"

class Mesh
{
public:
	std::vector<SimpleVertex> sim_vertices;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Shader shader;

	VAO VAO;
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	Mesh(std::vector<SimpleVertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

	void Draw(Camera& camera, glm::mat4 matrix = glm::mat4(1.f), glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f), glm::quat rotation = glm::quat(1.f, 0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f));
};