#include "RedBall.h"

RedBall::RedBall()
{

}

RedBall::RedBall(ModelLoad* copyModel)
{
	explodeModel = new ModelLoad(copyModel);
	explodeModel->transform.SetTranslation(glm::vec3(0, 15, 0));
	explodeModel->transform.SetScale(glm::vec3(0.5f));
	explodeModel->isVisible = false;
	modelsLoaded.push_back(explodeModel);

}

void RedBall::MakeItbigger(float deltaTime, std::vector<GreyBall* > &greyBallsList)
{
    if (explodeModel &&isSpawned)
    {
        float scaleFactor = 5.0f; // Factor to increase the scale
        float scaleIncreaseRate = 0.1f; // Rate at which the scale increases

        explodeModel->transform.scale += scaleFactor * scaleIncreaseRate * deltaTime;
		if (explodeModel->transform.scale.x >=2.5f)
		{
			if (isCollidedWithShip && greyBallDecal)
			{
			
				greyBallDecal->greyModel->transform.position = explodeModel->transform.position;
				greyBallDecal->greyModel->isVisible = true;
				//isCollidedWithShip = false;
				greyBallsList.push_back(greyBallDecal);
			}
	
			isSpawned = false;
			explodeModel->isVisible = false;

		}
    }
}

RedBall::~RedBall()
{

}

void RedBall::removeRedBall()
{
	greyBallDecal->removeGreyBall();
	modelsLoaded.erase(std::remove(modelsLoaded.begin(), modelsLoaded.end(), explodeModel), modelsLoaded.end());

}
