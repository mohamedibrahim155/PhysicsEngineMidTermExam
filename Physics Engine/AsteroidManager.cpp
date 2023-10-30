#include "AsteroidManager.h"

AsteroidManager::AsteroidManager()
{

}

AsteroidManager::~AsteroidManager()
{
}

void AsteroidManager::SpawnInRandomPos(ModelLoad* AsteroidOne, ModelLoad* Ship)
{
	int asteroidcount = 20;
	for (size_t i = 0; i < asteroidcount; i++)
	{


		int PosX = randomNumberGen(-30, 30);
		int PosY = randomNumberGen(-30, 30);
		int PosZ = randomNumberGen(-30, 30);

		int randomOffsetX = randomNumberGen(-1, 1);
		int randomOffsetY = randomNumberGen(-1, 1);
		int randomOffsetZ = randomNumberGen(-1, 1);
		std::shared_ptr<Asteroid> asteroidCopy  = std::make_shared<Asteroid>(AsteroidOne);
		asteroidCopy->model->transform.position = glm::vec3(PosX, PosY, PosZ);
		glm::vec3 direction = glm::normalize(Ship->transform.position - asteroidCopy->model->transform.position);
		direction +=  glm::vec3(randomOffsetX, randomOffsetY, randomOffsetZ);
		asteroidCopy->DistanceFromShip = Ship->transform.position;
		asteroidCopy->phys->velocity = direction *50.0f;
		asteroidCopy->setPhysicsEngine(physEngine);
		asteroidCopy->redball = new RedBall(redBallObj);
		asteroidCopy->redball->greyBallDecal = new GreyBall(greyBallObj);

		asteroidModels.push_back(asteroidCopy);
		physEngine->AddPhysicsObjects(asteroidCopy->getPhysicsObject());

	}

}

void AsteroidManager::SetCollisionCallBackForAsteroids()
{

}

void AsteroidManager::setEngine(PhysicsEngine& engine)
{
	this->physEngine = &engine;
}
void AsteroidManager::ScaleFactorRedBall(float deltaTime)
{

	int MAXDISTANCE = 30;
	//if (asteroidModels.size()>0)
	{
		for (size_t i = 0; i < asteroidModels.size(); i++)
		{
			if (asteroidModels[i]->redball->explodeModel)
			{
				if (asteroidModels[i]->redball->isSpawned)
				{
					asteroidModels[i]->redball->MakeItbigger(deltaTime);

				}
			}

			glm::vec3 startPoint = asteroidModels[i]->DistanceFromShip;
			glm::vec3 current = asteroidModels[i]->model->transform.position;
			float dist = glm::distance(startPoint, current);
			if (dist > MAXDISTANCE)
			{
				asteroidModels[i]->model->isVisible = false;

				asteroidModels[i]->RemoveAsteroid();
				asteroidModels.erase(asteroidModels.begin() + i);
			}
		}
	}



//	std::cout << "ModelLoaded list : " << asteroidModels.size() << std::endl;

	//std::cout << "aestroid list : " << asteroidModels.size() << std::endl;

}
int AsteroidManager::randomNumberGen(int min, int max)
{

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max - 1);
	int random_number = dis(gen);
	return random_number;

}




