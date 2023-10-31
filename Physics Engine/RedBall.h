#pragma once
#include "Model.h"
#include "GreyBall.h"
extern std::vector<ModelLoad*> modelsLoaded;
class RedBall
{
public:
	RedBall();
	RedBall(ModelLoad* copyModel);
	void MakeItbigger(float deltaTime, std::vector<GreyBall* > &greyBallsList);
	~RedBall();
	ModelLoad* explodeModel;
	GreyBall* greyBallDecal;
	bool isSpawned = false;
	bool isCollidedWithShip = false;
	void removeRedBall();

private:

};