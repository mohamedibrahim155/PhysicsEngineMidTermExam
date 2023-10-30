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
	
	void SpawnInRandomPos(ModelLoad* AsteroidOne, ModelLoad* Ship);
	void SetCollisionCallBackForAsteroids();
	int randomNumberGen(int min, int max);
	PhysicsEngine* physEngine;
	ModelLoad* redBallObj;
	ModelLoad* greyBallObj;
	//RedBall* redSpawnBall;
	void setEngine(PhysicsEngine& engine);
	void ScaleFactorRedBall(float deltaTime);
private:

};

