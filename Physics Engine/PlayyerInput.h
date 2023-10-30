#pragma once
#include <iostream>
#include "OpenGL_Common.h"
#include<glm/glm.hpp>
#include "Model.h"
#include "PhysicsObject.h"
#include <string>
class PlayerInput
{
public:
	PlayerInput();
	~PlayerInput();

	int health = 100;
	PhysicsObject* playerPhysics;
	ModelLoad* playerModel;
	std::string playerName = "Player";
	void MovePlayer(GLFWwindow* window, glm::vec3& pos);

	bool Health();

private:

	class InputHandler;

	InputHandler* inputPtr;


};



