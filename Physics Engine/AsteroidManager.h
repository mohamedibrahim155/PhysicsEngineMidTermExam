#pragma once
#include"Asteroid.h"
#include "PhysicsEngine.h";
#include <random>
#include "RedBall.h"
#include<glm/glm.hpp>
class AsteroidManager
{
public:
	AsteroidManager();
	~AsteroidManager();
	std::vector<std::shared_ptr<Asteroid> > asteroidModels;
	std::vector<GreyBall* > greyBallsList;
	
	void SpawnInRandomPos(ModelLoad* AsteroidOne, ModelLoad* Ship);
	void ChangeAstroidsDirections(float deltaTime);
	void SetCollisionCallBackForAsteroids();
	int randomNumberGen(int min, int max);
	PhysicsEngine* physEngine;
	ModelLoad* redBallObj;
	ModelLoad* greyBallObj;
	void setEngine(PhysicsEngine& engine);
	void ScaleFactorRedBall(float deltaTime);
private:

};

