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

		while (PosX<20 && PosY < 20 && PosZ< 20)
		{
			PosX = randomNumberGen(-30, 30);
			PosY = randomNumberGen(-30, 30);
			PosZ = randomNumberGen(-30, 30);

		}
		int randomOffsetX = randomNumberGen(-1, 1);
		int randomOffsetY = randomNumberGen(-1, 1);
		int randomOffsetZ = randomNumberGen(-1, 1);
		std::shared_ptr<Asteroid> asteroidCopy  = std::make_shared<Asteroid>(AsteroidOne);
		asteroidCopy->model->transform.position = glm::vec3(PosX, PosY, PosZ);
		glm::vec3 direction = glm::normalize(Ship->transform.position - asteroidCopy->model->transform.position);
		direction +=  glm::vec3(randomOffsetX, randomOffsetY, randomOffsetZ);
		asteroidCopy->DistanceFromShip = Ship->transform.position;
		asteroidCopy->phys->velocity = direction *15.0f;
		asteroidCopy->setPhysicsEngine(physEngine);
		asteroidCopy->redball = new RedBall(redBallObj);
		asteroidCopy->redball->greyBallDecal = new GreyBall(greyBallObj);

		asteroidModels.push_back(asteroidCopy);
		physEngine->AddPhysicsObjects(asteroidCopy->getPhysicsObject());

	}

}

void AsteroidManager::ChangeAstroidsDirections(float deltaTime)
{
	for (size_t i = 0; i < asteroidModels.size(); i++)
	{
		//glm::vec3 startPoint = Ship->transform.position;
		glm::vec3 startPoint = asteroidModels[i].get()->DistanceFromShip;

		glm::vec3 currentDir = asteroidModels[i].get()->model->transform.position;
		glm::vec3 nomralizeDir = glm::normalize(currentDir);

		int randomOffsetX = randomNumberGen(-1, 1);
		int randomOffsetY = randomNumberGen(-1, 1);
		int randomOffsetZ = randomNumberGen(-1, 1);
		currentDir += glm::vec3(randomOffsetX, randomOffsetY, randomOffsetZ);
		glm::vec3 current = currentDir;
		float dist = glm::distance(current, startPoint);
		if (dist < 18)
		{
			asteroidModels[i].get()->phys->velocity = nomralizeDir * 15.0f;

		}

	}


	if (greyBallsList.size() > 0)
	{
		
		for (size_t i = 0; i < greyBallsList.size(); i++)
		{
			glm::vec3 startPos = greyBallsList[i]->greyModel->transform.position;
			glm::vec3 nomralizeDir = glm::normalize(startPos);
			greyBallsList[i]->greyModel->transform.position += nomralizeDir * deltaTime * 20.0f;
			glm::vec3 current = greyBallsList[0]->greyModel->transform.position;

		
		}


		for (size_t i = 0; i < greyBallsList.size(); i++)
		{

			glm::vec3 current = greyBallsList[i]->greyModel->transform.position;
			float dis = glm::distance(glm::vec3(0), current);
			if (dis > 80)
			{
				
				greyBallsList.erase(greyBallsList.begin() + i);

			}

		}
	

		std::cout << "Greyballs list : " << greyBallsList.size() << std::endl;
		
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
					asteroidModels[i]->redball->MakeItbigger(deltaTime,greyBallsList);

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






}
int AsteroidManager::randomNumberGen(int min, int max)
{

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max - 1);
	int random_number = dis(gen);
	return random_number;

}




