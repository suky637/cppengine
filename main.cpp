
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include "Mesh.h"
#include "Model.h"
#include "ray.h"
#include "PhysicManager.h"

// importing networking


int main()
{



	// CHANGE IN MULTIPLAYER
	unsigned const int MAX_HEALTH = 100;
	unsigned int health = MAX_HEALTH;
	unsigned int ammo = 200;
	unsigned int cur_scene = 0;
	unsigned int player_id = 0;

	ma_engine engine;
	if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
		return -1;

	int width = 1280;
	int height = 720;

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

	Camera camera(width, height, glm::vec3(0.0f, 10.0f, 0.f));
	// Take care of all the light related things
	
	

	Model model("Res/plane.json");
	Model skybox("Res/skybox.json");
	Model pistol("Res/machine_gun.json");
	const std::string PLAYER_PATH = "Res/player.json";
	std::map<std::string, Model> playerTest{};

	asio::error_code ec;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint;
	asio::ip::tcp::socket socket(context);

	json JSON;

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object

	glm::vec3 downPos = camera.Position;
	downPos.y -= 1;
	
	PhysicManager physics{};

	// ADDING COLLIDERS
	BoxCollider col = { glm::vec3(-10,-1,-10), glm::vec3(20, 1,20) };
	physics.addCollider(col);

	const double FIXED_DELTA_TIME = 1 / 60;

	double lastTime = glfwGetTime();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	double lastMouseX;
	double lastMouseY;
	glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
	bool isDone = false;


	//pistol.model = glm::translate(pistol.model, glm::vec3(0, 0, 0.2f));

	glUniformMatrix4fv(pistol.meshes[0].shader.getLoc("model2"), 
		1,
		GL_FALSE, 
		glm::value_ptr(
			glm::translate(glm::vec3(0.f, 0.f, 0.3f)
	)));



	double time = glfwGetTime();
	double last_time_ = glfwGetTime();
	double last_time__ = glfwGetTime();
	double deltaTime = 0;
	double FIRERATE = 0.2;
	double fireRateTime = 0;

	std::vector<std::string> players{};

	while (!glfwWindowShouldClose(window))
	{


		glfwPollEvents();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (cur_scene == 1)
		{

			double curTime = glfwGetTime();


			time = glfwGetTime();

			deltaTime = time - lastTime;
			glm::quat rot = glm::quatLookAt(camera.Orientation, camera.Up);

			float ang = glm::eulerAngles(rot).y;

			fireRateTime += deltaTime;

			if (curTime - lastTime >= 1 / 60)
			{

				glm::vec4 off{camera.Position.x + 0.1f, camera.Position.y - 0.1f, camera.Position.z - 0.3f, 1.f};
				glm::vec3 off2{camera.Position.x, camera.Position.y, camera.Position.z};
				pistol.rotations[0] = glm::mat4(1.f);
				glm::mat4 centre = glm::translate(glm::vec3(0.f, 0.f, 0.f));
				//pistol.model = glm::mat4(1.f);
				if (!ImGui::IsAnyItemActive())
				{
					ImGui::SetMouseCursor(0);
					camera.Inputs(window);

					if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
						fireRateTime >= FIRERATE &&
						ammo > 0)
					{
						ma_engine_play_sound(&engine, "Res/shoot.mp3", NULL);
						if (physics.Raycast(30, 1, camera.Orientation, camera.Position))
						{
							std::cout << "HIT\n";
						}
						fireRateTime = 0;
						ammo--;
					}

					isDone = false;
				}
				else {
					if (!isDone)
					{
						glfwSetCursorPos(window, lastMouseX, lastMouseY);
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
					isDone = true;
				}
				camera.updateMatrix(45.f, 0.1f, 100.f);



				// Physics
				downPos = glm::vec3(camera.Position.x, camera.Position.y - 1, camera.Position.z);
				pistol.rotations[0] = glm::rotate(centre, -camera.rotation.y, camera.Up) * glm::rotate(centre, camera.rotation.x, glm::vec3(-1, 0, 0));

				if (!physics.isPointCollides(downPos))
				{
					camera.gravity += 0.002f;

				}
				else {
					camera.gravity = 0.f;
					if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
					{
						camera.gravity = -0.07f;
					}
				}

				camera.velocity.y -= camera.gravity;
				// RENDER

				//glUniform3f(pistol.meshes[0].shader.getLoc("offset"), (camera.Position.x) * rot.x, camera.Position.y - 0.1f, (camera.Position.z) * rot.z);
				glm::vec3 vel = -camera.velocity;
				camera.ApplyVelocity();
				//pistol.translations[0] = glm::vec3((camera.Position.x + 0.1f) * 0.02f, (camera.Position.y - 0.1f) * 0.02f, -(camera.Position.z - 0.3f) * 0.02f);
				// doing random stuff with sines and cosines

				pistol.model = glm::translate(pistol.model, vel);

				lastTime = curTime;
				last_time_ = time;
			}
			if (curTime - last_time__ >= 1 / 12)
			{
				
				if (socket.is_open())
				{
					std::string data_to_send = "{\"id\": " + std::to_string(player_id) + ", \"pos\": [" + std::to_string(camera.Position.x) + ", " + std::to_string(camera.Position.y) + ", " + std::to_string(camera.Position.z) + "]}";
					//std::cout << data_to_send<<"\n";
					json JsonLeftData = json::parse(data_to_send);
					std::string req = "UPDT";
					req += JsonLeftData.dump();
					socket.write_some(asio::buffer(req.data(), req.size()), ec);
					socket.wait(socket.wait_read);
					size_t bytes = socket.available();
					//std::cout << "Bytes Available: " << bytes << std::endl;

					if (bytes > 0)
					{
						std::vector<char> vBuffer(bytes);
						socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

						std::string data = "";

						for (auto c : vBuffer)
							data += c;
						//std::cout << data << std::endl;
						JSON = json::parse(data);
						std::map<std::string, json> _map = JSON["players"];

						players.clear();

						for (auto const& [key, val] : _map)
						{
							players.push_back(key);
							if (std::to_string(player_id) != key)
							{
								bool hasBeenInitialized = true;
								for (auto const& [key2, val2] : playerTest)
								{
									if (key == key2)
									{
										hasBeenInitialized = false;
									}
								}
								if (!hasBeenInitialized)
								{
									playerTest[key] = Model(PLAYER_PATH.c_str());
								}
								//glm::vec3 plr(JSON["players"][key]["position"][0], JSON["players"][key]["position"][1], JSON["players"][key]["position"][2]);
								//glm::mat4 model = glm::translate(plr);
								//playerTest[key].model = model;
							}
						}
						
					}
				}

				last_time__ = curTime;
			}

			skybox.Draw(camera);
			model.Draw(camera);
			pistol.Draw(camera);
			for (auto [key, val] : playerTest)
			{
				

				val.Draw(camera);
			}

			ImGui::Begin("HUD");
			ImGui::Text(("Health: " + std::to_string(health)).c_str());
			ImGui::Text(("Ammo: " + std::to_string(ammo)).c_str());

			ImGui::End();

			ImGui::Begin("Players");
			for (std::string t : players)
			{
				ImGui::Text((t + " Position (" + std::to_string((float)JSON["players"][t]["position"][0]) + "; " + std::to_string((float)JSON["players"][t]["position"][1]) + "; " + std::to_string((float)JSON["players"][t]["position"][2]) + ")").c_str());
			}
			ImGui::End();

		}

		if (cur_scene == 0)
		{
			ImGui::Begin("FPS game");
			ImGui::Text("I HAVENT FOUND A TITLE FOR THE GAME PLEASE TELL ME ANY IDEAS IN THE COMMENTS FOR THE GAME TITLE THANKS!");

			ImGui::Text("TODO: add a server list");
			if (ImGui::Button("Join game"))
			{
				const char* server_addr = "127.0.0.1";
				unsigned int port = 7777;

				// MULTIPLAYER I GUESS
				
				endpoint = asio::ip::tcp::endpoint(asio::ip::make_address(server_addr, ec), port);

				socket.connect(endpoint, ec);
				if (!ec)
				{
					std::cout << "Failed to connect to the server address: \n" << ec.message() << std::endl;
					
				}
				else
				{
				}
				if (socket.is_open())
				{
					std::cout << "Successfully connected to server!" << std::endl;
					std::string req = "CONN";
					socket.write_some(asio::buffer(req.data(), req.size()), ec);

					socket.wait(socket.wait_read);

					size_t bytes = socket.available();
					std::cout << "Bytes Available: " << bytes << std::endl;

					if (bytes > 0)
					{
						std::vector<char> vBuffer(bytes);
						socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

						std::string data = "";

						for (auto c : vBuffer)
							data += c;
						std::cout << data << std::endl;

						JSON = json::parse(data);
						
						// THIS BREAK
						json playerDat = JSON["players"][to_string(JSON["id"])];
						camera.Position = glm::vec3(playerDat["position"][0], playerDat["position"][1], playerDat["position"][2]);
						camera.velocity = glm::vec3(playerDat["position"][0], playerDat["position"][1], playerDat["position"][2]);
						player_id = JSON["id"];
					}

					cur_scene = 1;
				}
			}

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);

		glfwGetWindowSize(window, &width, &height);
		if (lastW != width || lastH != height)
		{

			glViewport(0, 0, width, height);
			camera.Dimentions(width, height);

			lastW = width;
			lastH = height;
		}
		glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
	}

	// Disconect from server
	if (socket.is_open())
	{
		std::string d = "{\"id\": " + std::to_string(player_id) + "}";
		json JsonLeftData = json::parse(d);
		JsonLeftData["id"] = player_id;
		std::string req = "DISC";
		req += JsonLeftData.dump();
		socket.write_some(asio::buffer(req.data(), req.size()), ec);
		socket.wait(socket.wait_read);
		size_t bytes = socket.available();
		std::cout << "Bytes Available: " << bytes << std::endl;

		if (bytes > 0)
		{
			std::vector<char> vBuffer(bytes);
			socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

			std::string data = "";

			for (auto c : vBuffer)
				data += c;
			std::cout << data << std::endl;
		}
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();
}