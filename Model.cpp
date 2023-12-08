#include"Model.h"

Model::Model(const char* file)
{
	// Make a JSON object
	std::string text = readFile(file);
	JSON = json::parse(text);

	// Get the binary data
	Model::file = file;

	// Get the number of Meshes
	
	for (int i = 0; i < JSON.size(); i++)
	{
		bool hasNormalMap = false;
		std::vector<glm::vec3> vertices{};
		for (int j = 0; j < JSON[i]["Vertices"].size(); j++)
		{
			glm::vec3 vertex{JSON[i]["Vertices"][j][0], JSON[i]["Vertices"][j][1], JSON[i]["Vertices"][j][2]};
			vertices.push_back(vertex);
		}
		std::vector<GLuint> indices{};
		for (int j = 0; j < JSON[i]["Indices"].size(); j++)
		{
			
			GLuint indice = unsigned(JSON[i]["Indices"][j]);
			indices.push_back(indice);
		}

		std::vector<Texture> textures{};
		for (unsigned int j = 0; j < JSON[i]["Textures"].size(); j++)
		{
			std::string type = JSON[i]["Textures"][j]["type"];
			std::string uri = JSON[i]["Textures"][j]["uri"];
			if (type == "normal")
			{
				hasNormalMap = true;
			}
			Texture tex{ uri.c_str(), type.c_str(), j };
			textures.push_back(tex);
		}
		std::vector<glm::vec2> UVs{};
		for (int j = 0; j < JSON[i]["UVs"].size(); j++)
		{
			glm::vec2 uv{JSON[i]["UVs"][j][0], JSON[i]["UVs"][j][1]};
			UVs.push_back(uv);
		}
		std::vector<glm::vec3> normals = std::vector<glm::vec3>(vertices.size());

		//std::cout << indices.size() << std::endl;
		// Calculating triangles
		for (int j = 0; j < indices.size(); j += 3) {
			// Calculate face normal
			glm::vec3 v0 = vertices[indices[j]];
			glm::vec3 v1 = vertices[indices[j + 1]];
			glm::vec3 v2 = vertices[indices[j + 2]];

			glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

			// Add the face normal to each vertex normal
			normals[indices[j]] += normal;
			normals[indices[j + 1]] += normal;
			normals[indices[j + 2]] += normal;
		}

		// Normalize the accumulated normals
		for (int j = 0; j < normals.size(); ++j) {
			normals[j] = glm::normalize(normals[j]);
		}

		// Generate vectors and quaternions
		glm::vec3 translation{0.f};
		glm::quat rotation{1.f, 0.0, 0.0f, 0.0f};
		glm::vec3 scale{1.f};

		for (int j = 0; j < JSON[i]["Transforms"].size(); j++)
		{
			std::string type = JSON[i]["Transforms"][j]["type"];
			float x = JSON[i]["Transforms"][j]["val"][0];
			float y = JSON[i]["Transforms"][j]["val"][1];
			float z = JSON[i]["Transforms"][j]["val"][2];
			glm::vec3 transformation = glm::vec3(x, y, z);
			if (type == "translation")
			{
				translation = transformation;
			}
			if (type == "scale")
			{
				scale = transformation;
			}
			if (type == "rotation")
			{
				rotation.x = transformation.x;
				rotation.y = transformation.y;
				rotation.z = transformation.z;
			}
		}

		translations.push_back(translation);
		rotations.push_back(rotation);
		scales.push_back(scale);
		
		if (!hasNormalMap)
		{
			std::vector<Vertex> vertex_{};
			// Generate Vertex also

			for (int j = 0; j < vertices.size(); j++)
			{
				Vertex ver{ vertices[j], normals[j], glm::vec3{1.f,1.f,1.f}, UVs[j] };
				vertex_.push_back(ver);
			}


			Mesh mesh{ vertex_, indices, textures };
			meshes.push_back(mesh);
		}
		else
		{
			std::vector<SimpleVertex> vertex_{};
			// Generate Vertex also

			for (int j = 0; j < vertices.size(); j++)
			{
				SimpleVertex ver{ vertices[j], glm::vec3{1.f,1.f,1.f}, UVs[j] };
				vertex_.push_back(ver);
			}


			Mesh mesh{ vertex_, indices, textures };
			meshes.push_back(mesh);
		}
	}
}

void Model::Draw(Camera& camera)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(camera, model, translations[i], rotations[i], scales[i]);
	}
}