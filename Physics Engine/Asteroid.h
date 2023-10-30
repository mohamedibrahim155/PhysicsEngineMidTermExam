#pragma once

#include"Model.h"
#include"PhysicsObject.h"
#include "PhysicsEngine.h"
#include "RedBall.h"
extern std::vector<ModelLoad*> modelsLoaded;
class Asteroid
{
public:
	Asteroid();
	Asteroid(ModelLoad* copyModel);
	~Asteroid();
	ModelLoad* model;
	PhysicsObject* phys;
	PhysicsObject* getPhysicsObject();
	glm::vec3 DistanceFromShip;
	PhysicsEngine* engine;
	void RemoveAsteroid();
	void setPhysicsEngine(PhysicsEngine* engine);

	RedBall* redball;
private:

};

