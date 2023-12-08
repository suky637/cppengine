#include "Mesh.h"
#include "Model.h"


int main()
{
	int width = 800;
	int height = 800;

	int lastW = width;
	int lastH = height;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "C++ Engine", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);
	
	// Generates Shader object using shaders default.vert and default.frag
	//Shader shaderProgram("default.vert", "default.frag");

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	// Take care of all the light related things
	
	

	Model model("Res/cube.json");


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object

	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		double curTime = glfwGetTime();

		
		//shaderProgram.Activate();
		camera.Inputs(window);
		camera.updateMatrix(45.f, 0.1f, 100.f);
		

		// RENDER


		model.Draw(camera);
		
		glfwSwapBuffers(window);

		glfwGetWindowSize(window, &width, &height);
		if (lastW != width || lastH != height)
		{

			glViewport(0, 0, width, height);
			camera.Dimentions(width, height);

			lastW = width;
			lastH = height;
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}