#pragma once
#include "GameBitmap.h"
#include "GameObject.h"

class Wall : public GameObject {
	RECT collision_rect;	// 충돌 체크를 위한 사각형
	HDC wdc;
	float width, height;
public:
	Wall(HINSTANCE g_hinst, float x, float y,float width,float height);

	virtual void get_BoundingRect(RECT& rect);
	virtual void update();
	virtual void draw(const HDC& mem1dc);
};