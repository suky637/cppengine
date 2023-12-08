#pragma once

#include<json/json.hpp>
#include"Mesh.h"

using json = nlohmann::json;


class Model
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	Model(const char* file);

	void Draw(Shader& shader, Camera& camera);

	glm::mat4 model{1.f};
	

private:
	// Variables for easy access
	const char* file;
	std::vector<unsigned char> data;
	std::vector<Mesh> meshes{};

	std::vector<glm::vec3> translations{};
	std::vector<glm::vec3> scales{};
	std::vector<glm::quat> rotations{};

	json JSON;
};