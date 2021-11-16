#pragma once
#include "GameBitmap.h"
#include "GameObject.h"

class BulletObject : public GameObject {
	float spd;
	float dx, dy;
	RECT collision_rect;
	HDC bdc;
public:
	BulletObject(HINSTANCE g_hinst, float x, float y);

	void setDir(float _dx, float _dy);
	void setPos(float _x, float _y);
	void setActive();

	virtual void update(float fTimeElapsed);
	virtual void draw(const HDC& mem1dc);

	bool actived = false;
};
