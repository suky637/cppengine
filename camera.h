#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera
{
private:
	bool firstClick = true;
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 Forward = glm::vec3(0.f, 0.f, -1.f);
	glm::mat4 cameraMatrix = glm::mat4(1.f);
	glm::vec3 velocity = glm::vec3(0.f);
	glm::vec2 rotation{0.f};
	float gravity = 0.1f;

	int width;
	int height;

	float speed = 0.1f;
	float sensitivity = 100.f;

	Camera(int width, int height, glm::vec3 position);

	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);
	void Dimentions(int width, int height);
	void Inputs(GLFWwindow* window);
	void ApplyVelocity();
};