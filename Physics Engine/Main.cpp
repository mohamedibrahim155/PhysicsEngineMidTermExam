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
std::string Game_Title = "Model Loading";
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
bool isSheildActivated = false;
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
std::vector<ModelLoad*> modelsLoadedFromFile;
std::vector<ModelLoad*> bullets;

ModelLoad* tempBullet;
void Instantiate(ModelLoad* model);

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
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		switchCamera = !switchCamera;
		std::cout << "Value of swtich camera : " << switchCamera << std::endl;
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{

		switchToPointLight = !switchToPointLight;
		std::cout << "Value of swtich pointLight : " << switchToPointLight << std::endl;
		if (switchToPointLight)
		{
			currentLight = PointLightPos;
		}
		else
		{
			currentLight = spotlightPos;

		}
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		Instantiate(tempBullet);



	}

	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		WriteModelData("Save.txt");

	}
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







	ModelLoad* lightModel= new ModelLoad();
	lightModel->diffuseTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Diffuse.png", DIFFUSE);
	lightModel->specularTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Specular.png", SPECULAR);
	lightModel->LoadModel("Models/SpecSphere/Sphere 1.fbx");
	lightModel->transform.scale = glm::vec3(1);
	lightModel->modelName = "Player";
	lightModel->transform.scale = glm::vec3(0.3f);
	//modelsLoaded.push_back(lightModel);



	

	ModelLoad* explosionSphereModel = new ModelLoad();
	explosionSphereModel->diffuseTexture.LoadTexture("Models/SpecSphere/redColor.png", DIFFUSE);
	explosionSphereModel->specularTexture.LoadTexture("Models/SpecSphere/redColor.png", SPECULAR);
	explosionSphereModel->LoadModel("Models/DefaultSphere/Sphere_1_unit_Radius.ply");

	ModelLoad* DecalModel = new ModelLoad();
	DecalModel->diffuseTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Diffuse.png", DIFFUSE);
	DecalModel->specularTexture.LoadTexture("Models/SpecSphere/Sphere 1_Sphere__Specular.png", SPECULAR);
	DecalModel->LoadModel("Models/DefaultSphere/Sphere_1_unit_Radius.ply");

	

	ModelLoad* Ship = new ModelLoad();
	Ship->diffuseTexture.LoadTexture("Models/StarTrek/Diffuse.png", DIFFUSE);
	Ship->specularTexture.LoadTexture("Models/StarTrek/Specualar.png", SPECULAR);
	Ship->LoadModel("Models/StarTrek/ship.ply");
	Ship->modelName = "SHIP";
	//plane->transform.position.y = -1;
	Ship->transform.scale =glm::vec3(0.001f);
	//modelsLoaded.push_back(Ship);



	
		ModelLoad* AsteroidOne = new ModelLoad();
		AsteroidOne->diffuseTexture.LoadTexture("Models/StarTrek/Asteroids/Diffuse.jpg", DIFFUSE);
		AsteroidOne->specularTexture.LoadTexture("Models/StarTrek/Asteroids/Specular.jpg", SPECULAR);
		AsteroidOne->LoadModel("Models/StarTrek/Asteroids/AsteroidOne.ply");
		AsteroidOne->modelName = "ASTEROID";
		AsteroidOne->transform.position = glm::vec3(2,8,2);
		AsteroidOne->transform.scale = glm::vec3(0.0015f);
		//modelsLoaded.push_back(AsteroidOne);


		

	LightManager lightManager;
	Light directionLight;
	directionLight.lightType = DIRECTION_LIGHT;
	directionLight.lightModel = lightModel;
	directionLight.ambient = glm::vec3(0.3f);
	directionLight.diffuse = glm::vec3(0.3f);


	lightManager.AddNewLight(directionLight);
	lightManager.SetUniforms(defaultShader.ID);

	PhysicsEngine engine;

	AsteroidManager manager;
	//for (size_t i = 0; i < 5; i++)
	//{
	//	//ModelLoad* Asteroidcopy = new ModelLoad(AsteroidOne);
	//	//Asteroidcopy->transform.position = glm::vec3(i * 1, 10, i * 3);
	//	//Asteroidcopy->transform.scale = glm::vec3(0.0015f);
	//	//modelsLoaded.push_back(Asteroidcopy);

	//	//PhysicsObject* asteroidPhysics = new PhysicsObject(Asteroidcopy);
	//	//asteroidPhysics->physicsType = SPHERE;
	//	//asteroidPhysics->Initialize(false,true);

	//	//asteroidPhysics->DoCollisionCall([asteroidPhysics](PhysicsObject* other) {
	//	//	std::cout << "Other name: " << other->model->modelName << std::endl;
	//	//	});

	//	//engine.AddPhysicsObjects(asteroidPhysics);



	//	Asteroid* nw = new Asteroid(AsteroidOne);
	//	nw->model->transform.position = glm::vec3(i * 1, 10, 0);
	//	glm::vec3 direction = glm::normalize(Ship->transform.position - nw->model->transform.position);
	//	nw->phys->velocity = direction;
	//	manager.asteroidLists.push_back(nw);
	//	engine.AddPhysicsObjects(nw->getPhysicsObject());

	//}
	


	PhysicsObject* shipPhysics = new PhysicsObject(Ship);
	Ship->transform.position = glm::vec3(0, -3, 0.0f);
	shipPhysics->physicsType = TRIANGLE;
	shipPhysics->Initialize(true,false,STATIC);

	manager.greyBallObj = new ModelLoad(DecalModel);
	manager.redBallObj = new ModelLoad(explosionSphereModel);
	manager.setEngine(engine);
	manager.SpawnInRandomPos(AsteroidOne, Ship);


	/*PhysicsObject* asteroidPhysics = new PhysicsObject(AsteroidOne);
	asteroidPhysics->physicsType = SPHERE;
	asteroidPhysics->Initialize(false);*/
	

	engine.AddPhysicsObjects(shipPhysics);
	//engine.AddPhysicsObjects(asteroidPhysics);
	
	

	

	float currentFrame = static_cast<float>(glfwGetTime());
	lastFrame = currentFrame;

	static float timer = 0.0f;
	static int currentWave = 1;
	static float waveInterval = 3.0f;
	while (!glfwWindowShouldClose(window))
	{
		

		

		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



	

		ProcessInput(window);
		glClearColor(0.1, 0.1f, 0.1f, 1.0f);
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
	//std::cout << "Ship visbile: "<<	Ship->isVisible << std::endl;

		for (size_t i = 0; i < bullets.size(); i++)
		{
			bullets[i]->DrawMeshes(defaultShader);
		}
		
		if (lastFrame==0)
		{
			continue;
		}

		engine.Update(deltaTime);
		
		timer += deltaTime;

		if (timer >= waveInterval)
		{
			manager.SpawnInRandomPos(AsteroidOne, Ship);

			timer = 0.0f;
		}
		manager.ScaleFactorRedBall(deltaTime);
		if (isSpacePressed)
		{
			manager.ChangeAstroidsDirections();
			//glm::vec3 startPoint = Ship->transform.position;
			//for (size_t i = 0; i < manager.asteroidModels.size(); i++)
			//{
			//	//glm::vec3 startPoint = Ship->transform.position;
			//	glm::vec3 startPoint = manager.asteroidModels[i].get()->DistanceFromShip;

			//	glm::vec3 currentDir = manager.asteroidModels[i].get()->model->transform.position;
			//	glm::vec3 nomralizeDir = glm::normalize(currentDir);

			//	int randomOffsetX = manager.randomNumberGen(-1, 1);
			//	int randomOffsetY = manager.randomNumberGen(-1, 1);
			//	int randomOffsetZ = manager.randomNumberGen(-1, 1);
			//	currentDir += glm::vec3(randomOffsetX, randomOffsetY, randomOffsetZ);
			//	glm::vec3 current = currentDir;
			//	float dist = glm::distance(current, startPoint);
			//	if (dist<18)
			//	{
			//		manager.asteroidModels[i].get()->phys->velocity = nomralizeDir * 15.0f;

			//	}
			//	
			//}
		}
		//std::cout << "sphere position : " <<speherePhysics->UpdateSphere() << " " << sphere3->transform.position.y << " " << sphere3->transform.position.z<<std::endl;


		glfwSwapBuffers(window);
		glfwPollEvents();

	}



	

	modelsLoaded.clear();
	glfwTerminate();
	return 0;

}
int posCount = 0;
void Instantiate(ModelLoad* model)
{
	
	/*posCount++;
	ModelLoad* temp = new ModelLoad();
	temp->modelName = model->modelName;
	temp->transform.position = glm::vec3(posCount * -2.0f, 3.0, 2.0f);
	temp->ListofMeshesInthisModel = std::vector<MeshData>(model->ListofMeshesInthisModel.begin(), model->ListofMeshesInthisModel.end());
	bullets.push_back(temp);
	PhysicsObject bullet(temp);
	bullet.physicsType = SPHERE;
	bullet.Initialize(true);*/
	


}


void MakeAsteroids()
{

}

void Shoot(GLFWwindow* window, glm::vec3& pos, ModelLoad* instanceMesh, ShaderClass& shader)
{
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		pos = glm::vec3(0.0f);
		instanceMesh->DrawMeshes(shader);

		
		
	}
	


}