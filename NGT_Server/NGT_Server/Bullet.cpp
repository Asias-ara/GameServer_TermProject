#include "Bullet.h"

BulletObject::BulletObject()
{
	x = 0;
	y = 0;
	spd = 100;
	dx = 0;
	dy = 0;
	actived = false;
}

void BulletObject::setDir(float _dx, float _dy)
{
	dx = _dx;
	dy = _dy;
}

void BulletObject::setPos(float _x, float _y)
{
	x = _x;
	y = _y;

}

void BulletObject::setActive()
{
	actived = true;
}


float BulletObject::getPos_x()
{
	return x;
}
float BulletObject::getPos_y()
{
	return y;
}

bool BulletObject::getActive()
{
	return actived;
}

void BulletObject::update(float fTimeElapsed)
{
	x += spd * dx* fTimeElapsed;
	y += spd * dy* fTimeElapsed;
}