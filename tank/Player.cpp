#include "stdafx.h"
#include "Player.h"
#include "Wall.h"


Player::Player(HINSTANCE g_hinst, char id, float x, float y) 
	:GameObject(g_hinst,"img/tank.bmp",x,y)
{
	this->id = id;
	this->dx = 0;
	this->dy = 0;
	this->spd = 100;
	
	get_BoundingRect(collision_rect);
}

void Player::setObject(GameObject* other)
{
	this->other = (Wall*)other;
}

void Player::Move(DWORD dwDirection, float frame_time)
{
	if (dwDirection)
	{
		float dx = 0, dy = 0;
		if (dwDirection & DIR_FORWARD) dy -= spd* frame_time;
		if (dwDirection & DIR_BACKWARD) dy += spd * frame_time;
		if (dwDirection & DIR_RIGHT) dx += spd * frame_time;
		if (dwDirection & DIR_LEFT) dx -= spd * frame_time;

		this->dx = dx;
		this->dy = dy;

	}
}



void Player::draw(const HDC& mem1dc)
{
	GameObject::draw(mem1dc, collision_rect);

	if (bitmap)bitmap->draw(mem1dc, pdc, x, y);


}
void Player::update(float fTimeElapsed)
{

	get_BoundingRect(collision_rect); //플레이어의 바운딩박스를 업데이트
	RECT o_crect;
	other->get_BoundingRect(o_crect); //충돌검사할 다른객체의 바운딩박스를 가져옴
	
	//충돌이 있으면 이전위치로
	if (collide(o_crect, collision_rect)) {
		x = px;
		y = py;
	}
	else {//없으면 진행방향으로 이동
		px = x;
		py = y;
		x += dx;
		y += dy;
	}
	


	dx = 0;
	dy = 0;

}