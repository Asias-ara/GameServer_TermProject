#include "stdafx.h"
#include "Wall.h"

Wall::Wall(HINSTANCE g_hinst, float x, float y, float width, float height)
	:GameObject(g_hinst,x,y)
{
	this->width = width;
	this->height = height;
	get_BoundingRect(collision_rect);
}

void Wall::get_BoundingRect(RECT& rect)
{
	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;
}
void Wall::draw(const HDC& mem1dc)
{
	GameObject::draw(mem1dc, collision_rect);
}
void Wall::update()
{
	get_BoundingRect(collision_rect);
}
