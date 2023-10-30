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
				std::cout << "Other name: " << other->model->modelName << std::endl;
				phys->collisionCallbool = false;
				phys->collisionCallback = nullptr;
				redball->explodeModel->isVisible = true;
				model->isVisible = false;
				redball->explodeModel->transform.position = engine->collisionPoints[0];
				redball->isSpawned = true;
				redball->isCollidedWithShip = false;
			//	engine->RemovePhysicsObject(phys);
			}
			if (other->model->modelName == "SHIP")
			{
				std::cout << "Other name: " << other->model->modelName << std::endl;
				phys->collisionCallbool = false;
				phys->collisionCallback = nullptr;
				std::cout << "x : " << engine->collisionPoints[0].x << "y : " << engine->collisionPoints[0].y << "z : " << engine->collisionPoints[0].z << std::endl;
				//engine->RemovePhysicsObject(phys);
				redball->explodeModel->isVisible = true;
				model->isVisible = false;
				redball->explodeModel->transform.position = engine->collisionPoints[0];
				redball->isSpawned = true;
				redball->isCollidedWithShip = true;
			//	engine->RemovePhysicsObject(phys);
			
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
