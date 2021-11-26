#pragma once
#include "GameBitmap.h"
#include "GameObject.h"

class BG : public GameObject {
	HDC bgdc;
public:
	BG(HINSTANCE g_hinst,float x ,float y);
	~BG();

	virtual void update();
	virtual void draw(const HDC& mem1dc);
};