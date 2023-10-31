#include <iostream>
#include "OpenGL_Common.h"
#include<fstream>
#include<sstream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include<glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "Shader.h"

#include"Model.h";
//#include "Mesh.h";
#include "Camera.h"
#include "LightManager.h"
#include "Texture.h"
//#include<windows.h>
//#include "PhysicsObject.h"
#include "WeaponFactory.h"
#include "PlayyerInput.h"
#include "PhysicsEngine.h"
#include "Asteroid.h"
#include "AsteroidManager.h"
#include"RedBall.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



#pragma region Variables Declaration
const int WIDTH_WINDOW = 1200;
const int HEIGHT_WINDOW = 800;
std::string Game_Title = "Physic Midterm";
Camera camera(glm::vec3(32, -1.5f, 25));
#pragma endregion

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

bool switchCamera = false;
bool switchToPointLight = false;
bool isSpacePressed = false;
bool changeCollisionType = false;

glm::vec3 currentLight(1.0f, 0.0f, 1.0f);
glm::vec3 spotlightPos(1.0f, 0.0f, 1.0f);
glm::vec3 PointLightPos(-1.0f, 0.0f, 1.0f);

static const char* DIFFUSE = "DIFFUSE";
static const char* SPECULAR = "SPECULAR";

struct ModelInfo
{
	ModelLoad model;
	glm::mat4 transformation;
};
std::vector<ModelInfo> models;
std::vector<ModelLoad*> modelsLoaded;




unsigned int loadTexture(char const* path);
void MoveModels(ModelInfo& model, glm::vec3 position);

void ReadData(const std::string& fileName, std::vector<ModelLoad>& models, std::vector<Light>& lights, std::vector<Camera>& cameras);
bool WriteModelData(std::string saveFileName);
std::vector<ModelLoad*> ReadModelData(const std::string& filename);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{

		if (switchCamera)
		{
			float velocity = camera.MovementSpeed * deltaTime;
			currentLight += camera.Front * velocity;
		}
		else
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}


	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{

		if (switchCamera)
		{
			float velocity = camera.MovementSpeed * deltaTime;
			currentLight -= camera.Front * velocity;
		}
		else
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (switchCamera)
		{
			float velocity = camera.MovementSpeed * deltaTime;
			currentLight -= camera.Right * velocity;
		}
		else
		{
			camera.ProcessKeyboard(LEFT, deltaTime);

		}

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (switchCamera)
		{
			float velocity = camera.MovementSpeed * deltaTime;
			currentLight += camera.Right * velocity;

		}
		else
		{
			camera.ProcessKeyboard(RIGHT, deltaTime);

		}

	}

	if (switchToPointLight)
	{
		PointLightPos = currentLight;
	}
	else
	{
		spotlightPos = currentLight;
	}
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{



	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		// modelsLoadedFromFile = ReadModelData("Save.txt");

		//// Printing the read data to verify
		// for (const auto& model : modelsLoaded)
		// {
		//	 std::cout << "Model_path:" << model->modelPath << std::endl;
		//	 std::cout << "Position: { " << model->transform.position.x << ", " << model->transform.position.y << ", " << model->transform.position.z << " }" << std::endl;
		//	 std::cout << "Rotation: { " << model->transform.rotation.x << ", " << model->transform.rotation.y << ", " << model->transform.rotation.z << " }" << std::endl;
		//	 std::cout << "Scale: { " << model->transform.scale.x << ", " << model->transform.scale.y << ", " << model->transform.scale.z << " }" << std::endl;
		// }



		// ModelLoad* LightModel = new ModelLoad();
		// LightModel->diffuseTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Diffuse.png", DIFFUSE);
		// LightModel->specularTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Specular.png", SPECULAR);
		// LightModel->LoadModel(modelsLoaded[0]->modelPath);
		// LightModel->transform.position.x= -5.0f;
		// modelsLoaded.push_back(LightModel);

		std::vector<ModelLoad> models;
		std::vector<Light> lights;
		std::vector<Camera> cameras;

		std::string fileName = "SimpleData.txt"; // Replace with the path of your file
		ReadData(fileName, models, lights, cameras);

		// Printing the read data to verify
		for (const auto& model : models) {
			std::cout << "Model_name: " << model.modelName << std::endl;
			std::cout << "Model_Path: " << model.modelPath << std::endl;
			std::cout << "Position: { " << model.transform.position.x << ", " << model.transform.position.y << ", " << model.transform.position.z << " }" << std::endl;
			std::cout << "Rotation: { " << model.transform.rotation.x << ", " << model.transform.rotation.y << ", " << model.transform.rotation.z << " }" << std::endl;
			std::cout << "Scale: { " << model.transform.scale.x << ", " << model.transform.scale.y << ", " << model.transform.scale.z << " }" << std::endl;
		}

		for (const auto& light : lights) {
			std::cout << "Model_Path: " << light.lightModel->modelPath << std::endl;
			std::cout << "Light_Type: " << light.lightType << std::endl;
			std::cout << "Position: { " << light.lightModel->transform.position.x << ", " << light.lightModel->transform.position.y << ", " << light.lightModel->transform.position.z << " }" << std::endl;
			std::cout << "Rotation: { " << light.lightModel->transform.rotation.x << ", " << light.lightModel->transform.rotation.y << ", " << light.lightModel->transform.rotation.z << " }" << std::endl;
			std::cout << "Scale: { " << light.lightModel->transform.scale.x << ", " << light.lightModel->transform.scale.y << ", " << light.lightModel->transform.scale.z << " }" << std::endl;
		}

		for (const auto& camera : cameras) {
			std::cout << "Position: { " << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << " }" << std::endl;
			//std::cout << "Rotation: { " << camera.rotation.x << ", " << camera.rotation.y << ", " << camera.rotation.z << " }" << std::endl;
			//std::cout << "Scale: { " << camera.scale.x << ", " << camera.scale.y << ", " << camera.scale.z << " }" << std::endl;
		}


	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		isSpacePressed = !isSpacePressed;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		changeCollisionType != changeCollisionType;
	}

}
void Shoot(GLFWwindow* window, glm::vec3& pos, ModelLoad* instanceMesh, ShaderClass& shader);

std::vector<MeshData> meshTemp;


int main()
{

	if (!glfwInit())
	{
		// Initialization failed
		std::cout << "Initialization Failed";
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window = glfwCreateWindow(WIDTH_WINDOW, HEIGHT_WINDOW, Game_Title.c_str() , NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);


	
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	ShaderClass LightShader("VertexShader.vert", "FragmentShader.frag");
	ShaderClass defaultShader("Light_VertexShader.vert", "Light_FragmentShader.frag");






	//DirectionLight model
	ModelLoad* lightModel= new ModelLoad();
	lightModel->diffuseTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Diffuse.png", DIFFUSE);
	lightModel->specularTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Specular.png", SPECULAR);
	lightModel->LoadModel("Models/SpecSphere/Sphere 1.fbx");
	lightModel->transform.scale = glm::vec3(1);
	lightModel->modelName = "Player";
	lightModel->transform.scale = glm::vec3(0.3f);
	



	
	//Red ball Model
	ModelLoad* explosionSphereModel = new ModelLoad();
	explosionSphereModel->diffuseTexture.LoadTexture("Models/SpecSphere/redColor.png", DIFFUSE);
	explosionSphereModel->specularTexture.LoadTexture("Models/SpecSphere/redColor.png", SPECULAR);
	explosionSphereModel->LoadModel("Models/DefaultSphere/Sphere_1_unit_Radius.ply");
	//Grey ball model
	ModelLoad* DecalModel = new ModelLoad();
	DecalModel->diffuseTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Diffuse.png", DIFFUSE);
	DecalModel->specularTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Specular.png", SPECULAR);
	DecalModel->LoadModel("Models/DefaultSphere/Sphere_1_unit_Radius.ply");

	
	//Space Ship model
	ModelLoad* Ship = new ModelLoad();
	Ship->diffuseTexture.LoadTexture("Models/StarTrek/Diffuse.png", DIFFUSE);
	Ship->specularTexture.LoadTexture("Models/StarTrek/Specualar.png", SPECULAR);
	Ship->LoadModel("Models/StarTrek/ship.ply");
	Ship->modelName = "SHIP";
	//plane->transform.position.y = -1;
	Ship->transform.scale =glm::vec3(0.001f);




		//Asteroid type 1 model
		ModelLoad* AsteroidOne = new ModelLoad();
		AsteroidOne->diffuseTexture.LoadTexture("Models/StarTrek/Asteroids/Diffuse.jpg", DIFFUSE);
		AsteroidOne->specularTexture.LoadTexture("Models/StarTrek/Asteroids/Specular.jpg", SPECULAR);
		AsteroidOne->LoadModel("Models/StarTrek/Asteroids/AsteroidOne.ply");
		AsteroidOne->modelName = "ASTEROID";
		AsteroidOne->transform.position = glm::vec3(2,8,2);
		AsteroidOne->transform.scale = glm::vec3(0.0015f);

		//Asteroid type 2 model
		ModelLoad* Asteroid2 = new ModelLoad();
		Asteroid2->diffuseTexture.LoadTexture("Models/StarTrek/Asteroids/Diffuse.jpg", DIFFUSE);
		Asteroid2->specularTexture.LoadTexture("Models/StarTrek/Asteroids/Specular.jpg", SPECULAR);
		Asteroid2->LoadModel("Models/StarTrek/Asteroids/AsteroidTwo.ply");
		Asteroid2->modelName = "ASTEROID";
		Asteroid2->transform.position = glm::vec3(2, 8, 2);
		Asteroid2->transform.scale = glm::vec3(0.0015f);
		

	LightManager lightManager;
	Light directionLight;
	directionLight.lightType = DIRECTION_LIGHT;
	directionLight.lightModel = lightModel;
	directionLight.ambient = glm::vec3(0.3f);
	directionLight.diffuse = glm::vec3(0.3f);


	lightManager.AddNewLight(directionLight);
	lightManager.SetUniforms(defaultShader.ID);

	//Phyics engine which handles collision
	PhysicsEngine engine;

	//Astroid manager, which handles spawning astroids and its factors
	AsteroidManager manager;

	//Added physics to Space ship
	PhysicsObject* shipPhysics = new PhysicsObject(Ship);
	Ship->transform.position = glm::vec3(0, -3, 0.0f);
	shipPhysics->physicsType = changeCollisionType ? TRIANGLE : AABB; // if "K" pressed, the spaceship collsion type changes
	shipPhysics->Initialize(true,false,STATIC);


	manager.greyBallObj = new ModelLoad(DecalModel);
	manager.redBallObj = new ModelLoad(explosionSphereModel);
	manager.setEngine(engine);

	bool swithModels = true;
	manager.SpawnInRandomPos(swithModels ? AsteroidOne : Asteroid2, Ship);
	engine.AddPhysicsObjects(shipPhysics);
	

	float currentFrame = static_cast<float>(glfwGetTime());
	lastFrame = currentFrame;

	float timer = 0.0f;
	int currentWave = 1;
	float waveInterval = 3.0f;
	while (!glfwWindowShouldClose(window))
	{
		

		

		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



	

		ProcessInput(window);
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		defaultShader.UseProgram();
		lightManager.UpdateUniformValues(defaultShader.ID);
		defaultShader.SetFloat("material.shininess", 52.f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH_WINDOW / (float)HEIGHT_WINDOW, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		defaultShader.SetMat4("projection", projection);
		defaultShader.SetMat4("view", view);
		//sphere2->DrawMeshes(defaultShader);

		for (size_t i = 0; i < modelsLoaded.size(); i++)
		{
			modelsLoaded[i]->DrawMeshes(defaultShader);

		}
		Ship->DrawMeshes(defaultShader);


		engine.Update(deltaTime); //Physics update
		



		timer += deltaTime;
		if (timer >= waveInterval)
		{
			swithModels != swithModels;
			manager.SpawnInRandomPos(swithModels ? AsteroidOne : Asteroid2, Ship);  //Spawning astroids randomly
			timer = 0.0f;
		}


		manager.ScaleFactorRedBall(deltaTime); // scaling the collided Astroids as red color ball



		if (isSpacePressed)
		{
			// if space press, deflecting the astroids in a range within a radius as sheild also deflecting grey particles
			manager.ChangeAstroidsDirections(deltaTime);
		}
		


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	modelsLoaded.clear();
	glfwTerminate();
	return 0;

}

