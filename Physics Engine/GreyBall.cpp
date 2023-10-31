
#include"GreyBall.h"

GreyBall::GreyBall()
{
}

GreyBall::GreyBall(ModelLoad* copyModel)
{
	greyModel = new ModelLoad(copyModel);
	greyModel->transform.SetScale(glm::vec3(1));
	greyModel->isVisible = false;
	modelsLoaded.push_back(greyModel);
}

GreyBall::~GreyBall()
{
}

void GreyBall::removeGreyBall()
{
	modelsLoaded.erase(std::remove(modelsLoaded.begin(), modelsLoaded.end(), greyModel), modelsLoaded.end());

}
