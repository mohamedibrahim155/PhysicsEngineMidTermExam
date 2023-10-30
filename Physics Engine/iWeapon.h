#pragma once

class iWeapon
{
public:
	iWeapon();
	virtual ~iWeapon();


	virtual void Shoot() = 0;
	virtual int AmmoCount() = 0;





private:


};

iWeapon::iWeapon()
{
}

iWeapon::~iWeapon()
{
}