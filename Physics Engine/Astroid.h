#pragma once
#include"Model.h"
#include"PhysicsObject.h"
class Astroid
{
public:
	Astroid();
	~Astroid();

	ModelLoad* astroidModel;
	PhysicsObject* astroidPhysics;
private:

};
