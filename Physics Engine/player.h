#pragma once
#include <iostream>
#include "CharacterType.h"
#include "CharacterFactory.h"


class Player : public CharacterFactory
{
public:
	Player();
	~Player();



	 EnemyType* CreateEnemyType() override;
	 CharacterType* CreateCharacter() override;


private:

};

Player::Player()
{
}

Player::~Player()
{
}



 



