#include "Asteroid.h"

Asteroid::Asteroid()
{
}

Asteroid::Asteroid(ModelLoad* copyModel)
{
	model = new ModelLoad(copyModel);
	model->transform.SetScale(glm::vec3(0.001f));
	modelsLoaded.push_back(model);

	phys = new PhysicsObject(model);
	phys->physicsType = SPHERE;
	phys->Initialize(true,true,DYNAMIC);
	phys->bounciness = 0;

	phys->DoCollisionCall([this](PhysicsObject* other) 
		{
			if (other->model->modelName =="ASTEROID")
			{
				// using model name as tag to check collision with asteroid
				phys->collisionCallbool = false;
				phys->collisionCallback = nullptr;
				redball->explodeModel->isVisible = true;
				model->isVisible = false;
				redball->explodeModel->transform.position = engine->collisionPoints[0];
				redball->isSpawned = true;
				redball->isCollidedWithShip = false;
			}
			if (other->model->modelName == "SHIP")
			{
				// using model name as tag to check collision with spaceShip
				phys->collisionCallbool = false;
				phys->collisionCallback = nullptr;
				redball->explodeModel->isVisible = true;
				model->isVisible = false;
				redball->explodeModel->transform.position = engine->collisionPoints[0];
				redball->isSpawned = true;
				redball->isCollidedWithShip = true;
			
			}

			phys->mode = STATIC;
		});


}

Asteroid::~Asteroid()
{
	delete phys;
}

PhysicsObject* Asteroid::getPhysicsObject()
{
	return phys;
}

void Asteroid::RemoveAsteroid()
{
	engine->RemovePhysicsObject(phys);	
	redball->removeRedBall();
	modelsLoaded.erase(std::remove(modelsLoaded.begin(), modelsLoaded.end(), model), modelsLoaded.end());

}

void Asteroid::setPhysicsEngine(PhysicsEngine* physicsEngine)
{
	engine = physicsEngine;
}
