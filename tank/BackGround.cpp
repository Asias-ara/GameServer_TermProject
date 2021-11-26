#include "stdafx.h"
#include "BackGround.h"

BG::BG(HINSTANCE g_hinst, float x, float y) :GameObject(g_hinst,"img/map.bmp", x, y)
{
	float w_scale = (float)CLIENT_WIDTH / (float)bitmap->getWidth();
	float h_scale = (float)CLIENT_HEIGHT / (float)bitmap->getHeight();
	bitmap->set_scale(w_scale, h_scale);
}

BG::~BG()
{
}

void BG::update()
{
}

void BG::draw(const HDC& mem1dc)
{
	if(bitmap)bitmap->draw(mem1dc, bgdc, x, y);
}
