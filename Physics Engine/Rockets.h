#pragma once
#include <iostream>
#include "iWeapon.h"
class Rockets : public iWeapon
{
public:
	Rockets();
	~Rockets();

	void Shoot() override;
	int AmmoCount() override;


private:

	int rocketAmmoCount = 5;
};

Rockets::Rockets()
{
}

Rockets::~Rockets()
{
}

inline void Rockets::Shoot()
{

	std::cout << "Rockets are shooting" << std::endl;
}

inline int Rockets::AmmoCount()
{
	return rocketAmmoCount;
}
