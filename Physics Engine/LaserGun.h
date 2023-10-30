#pragma once
#include "iWeapon.h"
#include <iostream>

class LaserGun: public iWeapon
{
public:
	LaserGun();
	virtual ~LaserGun();

     void Shoot() override;
	 int AmmoCount() override;


private:

	int laserGunAmmo = 10;
};

LaserGun::LaserGun()
{
}

LaserGun::~LaserGun()
{
}

inline void LaserGun::Shoot()
{
	std::cout << "......Laser Gun Shoot(): " << std::endl;
}

inline int LaserGun::AmmoCount()
{

	return laserGunAmmo;

}
