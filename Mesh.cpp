#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) : shader{ Shader("Res/default.vert", "Res/default.frag") }
{
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shader.Activate();
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();

	VBO VBO(vertices);
	EBO EBO(indices);

	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	VAO.UnBind();
	VBO.UnBind();
	EBO.UnBind();
}

Mesh::Mesh(std::vector<SimpleVertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) : shader{ Shader("Res/default_normal.vert", "Res/default_normal.frag") }
{
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shader.Activate();
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Mesh::sim_vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;
	
	VAO.Bind();

	VBO VBO(vertices);

	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(SimpleVertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(SimpleVertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(SimpleVertex), (void*)(6 * sizeof(float)));

	VAO.UnBind();
	VBO.UnBind();

}

void Mesh::SetShader(std::string path)
{
	shader = Shader((path + ".vert").c_str(), (path + ".frag").c_str());
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shader.Activate();
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
}

void Mesh::Draw(Camera& camera, std::string shape, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{


	shader.Activate();
	VAO.Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
	unsigned int numNormal = 0;

	

	for (unsigned int i = 0; i < textures.size(); ++i)
	{

		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		else if (type == "normal")
		{
			num = std::to_string(numNormal++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}




	glUniform3f(shader.getLoc("camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	glm::mat4 trans = glm::mat4(1.f);
	glm::mat4 rot = glm::mat4(1.f);
	glm::mat4 sca = glm::mat4(1.f);

	trans = glm::translate(translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	glUniformMatrix4fv(shader.getLoc("translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(shader.getLoc("rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(shader.getLoc("scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(shader.getLoc("model"), 1, GL_FALSE, glm::value_ptr(matrix));
	glUniform3f(glGetUniformLocation(shader.ID, "camOrientation"), camera.Orientation.x, camera.Orientation.y, camera.Orientation.z);
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

	if (shape == "Triangle")
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
