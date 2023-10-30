#pragma once
#include "iWeapon.h"

#include <vector>
#include "LaserGun.h"
#include "Rockets.h"
class WeaponFactory
{
public:
	WeaponFactory();
	~WeaponFactory();


	iWeapon* CreateWeapon(int weaponID);



private:
	std::vector< iWeapon* > weaponsList;

};

WeaponFactory::WeaponFactory()
{
}

WeaponFactory::~WeaponFactory()
{
}

inline iWeapon* WeaponFactory::CreateWeapon(int weaponID)
{
	
	if (weaponID == 1)
	{
		
		return new LaserGun();
	}

	if (weaponID == 2)
	{

		return new Rockets();
	}

	return nullptr;

}
