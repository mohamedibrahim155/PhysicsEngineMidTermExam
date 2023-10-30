#include "PhysicsObject.h"
#include "Triangle.h"


std::vector<glm::vec3> collisionPoint;
std::vector<glm::vec3> collisionNormal;

PhysicsObject::PhysicsObject(ModelLoad* model)
{
	this->model = model;
	acceleration = glm::vec3(0.0f);
	

}

PhysicsObject::PhysicsObject()
{

	acceleration = glm::vec3(0.0f);

}

PhysicsObject::~PhysicsObject()
{
	//delete model;
}

void PhysicsObject::update(float deltatime)
{
	glm::vec3 gravity(0.0f, -9.81f* gravityValue, 0.0f);
	acceleration = gravity;
	velocity += acceleration * deltatime;
	model->transform.position += velocity * deltatime;
	

}

//bool PhysicsObject::checkCollision( PhysicsObject& other)
//{
//
//	
//	switch (physicsType)
//	{
//	case PhysicsType::AABB:
//		if (other.physicsType == AABB)
//		{
//			return CheckCOllisionAABBvsAABB(UpdateAABB(), other.UpdateAABB());
//
//		}
//		else
//		{
//			return CheckSphereAABBCollision(other.UpdateSphere(), UpdateAABB());
//		}
//		break;
//
//	case PhysicsType::SPHERE:
//
//		if (other.physicsType == AABB)
//		{
//			return CheckSphereAABBCollision(UpdateSphere(), other.UpdateAABB());
//		}
//		else if(other.physicsType == SPHERE)
//		{
//			return CheckSpherevsSphereCollision(UpdateSphere(), other.UpdateSphere());
//		}
//		else if(other.physicsType == TRIANGLE)
//		{
//			return CollisionSphereVsMeshOfTriangles(UpdateSphere(), other.model->transform.getModelMatrix(),
//				other.listoftriangles, collisionPoint,collisionNormal);
//
//		}
//
//		break;
//
//	case PhysicsType::TRIANGLE:
//
//		if (other.physicsType == TRIANGLE)
//		{
//			
//		}
//		else
//		{
//			return CheckSpherevsSphereCollision(UpdateSphere(), other.UpdateSphere());
//		}
//
//		break;
//	}
//
//
//
//	return false;
//}


bool PhysicsObject::checkCollision(PhysicsObject* other, std::vector<glm::vec3>& collisionPoints, std::vector<glm::vec3>& collisionNormals)

{



	switch (physicsType)
	{
	case PhysicsType::AABB:
		if (other->physicsType == AABB)
		{
			return CheckCOllisionAABBvsAABB(UpdateAABB(), other->UpdateAABB(),collisionPoints,collisionNormals);

		}
		else if(other->physicsType == SPHERE)
		{
			//return CheckSphereAABBCollision(other->UpdateSphere(), UpdateAABB());
			cSphere* updatedSphere = new cSphere(other->UpdateSphere());
			return CheckSphereVSAABBCollision(updatedSphere, UpdateAABB(), false, collisionPoints, collisionNormals);
		}
		break;
#pragma region SphereVS
	case PhysicsType::SPHERE:

		if (other->physicsType == AABB)
		{
			cSphere* updatedSphere = new cSphere(UpdateSphere());
			return CheckSphereVSAABBCollision(updatedSphere, other->UpdateAABB(),true,collisionPoints,collisionNormals);
		}
		else if (other->physicsType == SPHERE)
		{
			cSphere* updatedSphere = new cSphere(UpdateSphere());
			cSphere* OtherupdatedSphere = new cSphere(other->UpdateSphere());
			return CheckSphereVSSphereCollision(updatedSphere, OtherupdatedSphere,collisionPoints,collisionNormals);
		}
		else if (other->physicsType == TRIANGLE)
		{

			return CollisionSphereVsMeshOfTriangles(UpdateSphere(), other->model->transform.getModelMatrix(),
				other->listoftriangles, collisionPoints, collisionNormals);

		}
#pragma endregion
	case PhysicsType:: TRIANGLE :
		if (other->physicsType == SPHERE)
		{
			
		}

		break;

	//case PhysicsType::TRIANGLE:

	//	if (other->physicsType == TRIANGLE)
	//	{

	//	}
	//	else if (other->physicsType == SPHERE)
	//	{
	//		return CheckSpherevsSphereCollision(UpdateSphere(), other->UpdateSphere());
	//	}

		break;
	}



	return false;
}

cAABB  PhysicsObject:: UpdateAABB()
{

	//cAABB temp;

	glm::vec3 originalMinV = aabb.minV;
	glm::vec3 originalMaxV = aabb.maxV;
	glm::mat4 transformMatrix = model->transform.getModelMatrix();
	glm::vec4 transformedMinV = transformMatrix * glm::vec4(originalMinV, 1.0f);
	glm::vec4 transformedMaxV = transformMatrix * glm::vec4(originalMaxV, 1.0f);

	return  cAABB(glm::vec3(transformedMinV), glm::vec3(transformedMaxV));
	
}

cSphere PhysicsObject::UpdateSphere()
{
	glm::vec3 originalCenter = sphereShape.center;
	float orginalRadius = sphereShape.radius;
	glm::mat4 transformMatrix = model->transform.getModelMatrix();
	glm::vec4 transformedCenter = transformMatrix * glm::vec4(originalCenter, 1.0f);

	glm::mat4 scaleMatrix = glm::mat4(1.0f); // Initialize the scale matrix
	scaleMatrix[0][0] = model->transform.scale.x;
	scaleMatrix[1][1] = model->transform.scale.y;
	scaleMatrix[2][2] = model->transform.scale.z;

	float maxScale = max(model->transform.scale.x, max(model->transform.scale.y, model->transform.scale.z));
	float updatedRadius = orginalRadius * maxScale;

	return cSphere(glm::vec3(transformedCenter), updatedRadius);
}

void PhysicsObject::CalculateTriangle()
{

	
	for  (MeshData* mesh : model->ListofMeshesInthisModel )
	{
		std::vector<Triangle> trianglelist;

		for (const Triangles& triangle : mesh->triangle)
		{
			Triangle temp;
			temp.v1 = triangle.v1;
			temp.v2 = triangle.v2;
			temp.v3 = triangle.v3;
			temp.normal = triangle.normal;

			trianglelist.push_back(temp);
		}
		
		listoftriangles.push_back(trianglelist);

	}

	
}

 

void PhysicsObject::Initialize(bool isKinematic, bool collision)
{
	this->isKinematic = isKinematic;
	this->collisionCallbool = collision;

	if (this->isKinematic)
	{
		gravityValue = 0;
	}
	else
	{
		gravityValue = 0.1f;
	}
	
	
		aabb = CalculateModelAABB();
		

	

	if (physicsType == SPHERE)
	{
		glm::vec3 position = (aabb.minV + aabb.maxV) * 0.5f;
		glm::vec3 sideLengths = aabb.maxV - aabb.minV;
		float radius = 0.5f * max(sideLengths.x, max(sideLengths.y, sideLengths.z));
		sphereShape =  cSphere(position, radius);
	}

	if (physicsType == TRIANGLE)
	{
		CalculateTriangle();
	}
	
}


cAABB PhysicsObject::CalculateModelAABB()
{
	if (model->ListofMeshesInthisModel.empty())
	{
		return cAABB{ glm::vec3(0.0f), glm::vec3(0.0f) };
	}

	cAABB minMax;

	minMax.minV = model->ListofMeshesInthisModel[0]->Vertices[0].vPos;
	minMax.maxV = model->ListofMeshesInthisModel[0]->Vertices[0].vPos;

	for (MeshData* mesh : model->ListofMeshesInthisModel)
	{
		cAABB temp = CalculateAABB(mesh->Vertices);

		minMax.minV.x = min(temp.minV.x, minMax.minV.x);
		minMax.minV.y = min(temp.minV.y, minMax.minV.y);
		minMax.minV.z = min(temp.minV.z, minMax.minV.z);

		minMax.maxV.x = max(temp.maxV.x, minMax.maxV.x);
		minMax.maxV.y = max(temp.maxV.y, minMax.maxV.y);
		minMax.maxV.z = max(temp.maxV.z, minMax.maxV.z);
	}

	return cAABB{ minMax.minV, minMax.maxV };
}

glm::vec3 PhysicsObject::GetPosition()
{
	return model->transform.position;
}

void PhysicsObject::SetPosition(const glm::vec3& Position)
{
	model->transform.position = Position;
}

const std::function<void(PhysicsObject*)>& PhysicsObject::GetCollisionCall()
{
	return collisionCallback;
}

void PhysicsObject::DoCollisionCall(const std::function<void(PhysicsObject*)>& collisionCallback)
{
	this->collisionCallback = collisionCallback;
}
