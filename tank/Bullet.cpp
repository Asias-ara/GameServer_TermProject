#include "stdafx.h"
#include "Bullet.h"

BulletObject::BulletObject(HINSTANCE g_hinst, float x, float y)
	:GameObject(g_hinst,"img/bullet.bmp",x,y)
{
	id = 0;
	spd = 100;
	dx = 0;
	dy = 0;
	get_BoundingRect(collision_rect);

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

void BulletObject::setActive(bool _act)
{
	actived = _act;
}

void BulletObject::setId(int client_id)
{
	id = client_id;
}

int BulletObject::getId()
{
	return id;
}
bool BulletObject::getActive()
{
	return actived;
}


void BulletObject::update(float fTimeElapsed)
{
	get_BoundingRect(collision_rect);

	if (actived)
	{
		x += spd * dx* fTimeElapsed;
		y += spd * dy* fTimeElapsed;
	}
}

void BulletObject::draw(const HDC& mem1dc)
{
	if (actived)
	{
		GameObject::draw(mem1dc, collision_rect);
		if (bitmap) bitmap->draw(mem1dc, bdc, x, y);
	}
	
}
