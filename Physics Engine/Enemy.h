#pragma once
#include <iostream>
#include "EnemyTyper.h"
#include "CharacterFactory.h"


class Enemy : public CharacterFactory
{
public:
	Enemy();
	~Enemy();



	CharacterType* CreateCharacter() override;
	EnemyType* CreateEnemyType() override;


private:

};

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

inline CharacterType* Enemy::CreateCharacter()
{
	return nullptr;
}

inline EnemyType* Enemy::CreateEnemyType()
{

	return new EnemyType();
}



