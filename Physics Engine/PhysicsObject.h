#pragma once
#include "Model.h"
#include "PhysicsCollisionTypes.h"
#include <functional>

enum PhysicsType
{
	SPHERE,
	AABB,
	TRIANGLE,
	//MESH
};





class PhysicsObject
{
public:
	PhysicsObject();
	PhysicsObject(ModelLoad* model);
	~PhysicsObject();

	ModelLoad* model;
	PhysicsType physicsType;
	glm::vec3 position = glm::vec3(0.0f);
	bool isKinematic;
	bool collisionCallbool;
	void update(float deltatime);
	cAABB UpdateAABB();
	cSphere UpdateSphere();
	void CalculateTriangle();
	std::function<void(PhysicsObject*)> collisionCallback = nullptr;
	std::vector<std::vector<Triangle>> listoftriangles;

	//bool checkCollision( PhysicsObject& other);
	bool checkCollision(PhysicsObject* other, std::vector<glm::vec3>& collisionPoints, std::vector<glm::vec3>& collisionNormals);
	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 acceleration;
	float gravityValue;
	void Initialize(bool isKinematic, bool collision = false);
	cAABB aabb;
	cSphere sphereShape;


	cAABB CalculateModelAABB();
	
	glm::vec3 GetPosition();

	void SetPosition(const glm::vec3& Position);
	const std::function<void(PhysicsObject*)>& GetCollisionCall();
	void DoCollisionCall(const std::function<void(PhysicsObject*)>& collisionCallback);

	float bounciness = 0.8f;

};



