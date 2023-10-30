#pragma once
#include"Model.h"

extern std::vector<ModelLoad*> modelsLoaded;
class GreyBall
{
public:
	GreyBall();
	GreyBall(ModelLoad* copyModel);
	~GreyBall();
	ModelLoad* greyModel;
	void removeGreyBall();
private:

};

