#include "stdafx.h"
#include "Wall.h"

Wall::Wall(HINSTANCE g_hinst, float x, float y, float width, float height)
	:GameObject(g_hinst,x,y)
{
	//bitmap->set_scale(30.0f / bitmap->getWidth(), 30.0f / bitmap->getHeight());

	this->width = width;
	this->height = height;
	get_BoundingRect(collision_rect);
}

void Wall::ImagetoScreen()
{
	x = x /** CLIENT_WIDTH / 1024*/ + bitmap->getWidth() * 0.5f;
	y = y /** CLIENT_HEIGHT / 1024*/ + bitmap->getHeight() * 0.5f;
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
	//x : ? = CLIENT : 1024
	//289 : 1024 = ? : CLIENT
	GameObject::draw(mem1dc, collision_rect);
	//if(bitmap)bitmap->draw(mem1dc, wdc, x , y );
}
void Wall::update()
{
	get_BoundingRect(collision_rect);
}
