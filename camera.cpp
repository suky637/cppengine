#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
	velocity = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 proj = glm::mat4(1.f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	proj = glm::perspectiveFov(glm::radians(FOVdeg), (float)width, (float)height, nearPlane, farPlane);

	cameraMatrix = proj * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(shader.getLoc(uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Dimentions(int width, int height)
{
	Camera::width = width;
	Camera::height = height;
}

void Camera::Inputs(GLFWwindow* window)
{

	glm::vec3 orientWithoutY = glm::vec3(Orientation.x, 0, Orientation.z);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		velocity += speed * orientWithoutY;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		velocity += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		velocity -= speed * orientWithoutY;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		velocity += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		
		//Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		//Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		speed = 0.3f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		
		float rotx = sensitivity * (float)(mouseY - (height / 2)) / height;
		float roty = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotx), glm::normalize(glm::cross(Orientation, Up)));

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
			rotation.x += glm::radians(rotx);
		}

		Orientation = glm::rotate(Orientation, glm::radians(-roty), Up);
		rotation.y += glm::radians(roty);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
	
}

void Camera::ApplyVelocity()
{
	Position += velocity;
	velocity = glm::vec3(0.f, 0.f, 0.f);
}

