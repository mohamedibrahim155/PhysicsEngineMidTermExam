
#include "Astroid.h"

extern std::vector<ModelLoad*> modelsLoaded;



Astroid::Astroid()
{
}

Astroid::Astroid( ModelLoad* copyModel)
{
	this->astroidModel = new ModelLoad(copyModel);
	this->astroidModel->transform.scale = glm::vec3(0.0015f);
	modelsLoaded.push_back(astroidModel);

	this->astroidPhysics = new PhysicsObject(astroidModel);
	this->astroidPhysics->physicsType = SPHERE;
	this->astroidPhysics->Initialize(false);
}

Astroid::~Astroid()
{

}

void Astroid::loadModel(ModelLoad* copyModel)
{
	this->astroidModel = new ModelLoad(copyModel);

	this->astroidModel->transform.scale = glm::vec3(0.0015f);
	modelsLoaded.push_back(this->astroidModel);

	this->astroidPhysics = new PhysicsObject(this->astroidModel);
	this->astroidPhysics->physicsType = SPHERE;
	this->astroidPhysics->Initialize(false);
}

PhysicsObject* Astroid::getAsteroidPhysics()
{
	return astroidPhysics;
}
