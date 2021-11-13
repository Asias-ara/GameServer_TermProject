#include "stdafx.h"
#include "Wall.h"

Wall::Wall(HINSTANCE g_hinst, float x, float y)
	:GameObject(g_hinst,"img/wall.bmp",x,y)
{
	get_BoundingRect(collision_rect);
}


void Wall::draw(const HDC& mem1dc)
{
	GameObject::draw(mem1dc, collision_rect);
	if(bitmap)bitmap->draw(mem1dc, wdc, x, y);
}
void Wall::update()
{
	get_BoundingRect(collision_rect);
}
