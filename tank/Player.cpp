#include "stdafx.h"
#include "Player.h"
#include "Wall.h"
#include "Network.h"

Player::Player(HINSTANCE g_hinst, char id, float x, float y) 
	:GameObject(g_hinst,"img/tank.bmp",x,y)
{
	this->id = id;
	dx = 0;
	dy = 0;
	spd = 100;
	GetCursorPos(&CursorPos);
	get_BoundingRect(collision_rect);

	for (int i = 0; i < BLTS; ++i)
	{
		bullets[i] = new BulletObject(g_hinst, x, y);
	}
}

Player::~Player()
{
	if (other) other = NULL;
	//if (bullets) delete bullets;
}

void Player::setPrevPos()
{
	x = px;
	y = py;
}
void Player::setObject(GameObject** other)
{
	this->other = (other);
}

void Player::fire()
{
	BulletObject* bull = NULL;
	for (int i = 0; i < BLTS; ++i)
	{
		if (!bullets[i]->actived)
		{
			bull = bullets[i];
			break;
		}
	}
	
	POINT pos{ x,y };
	if (bull)
	{
		float dx, dy;
		Normalize(pos, CursorPos,dx,dy);
		bull->setPos(x,y);
		bull->setDir(dx, dy);
		bull->setActive();
	}
}

void Player::Move(DWORD dwDirection, float frame_time)
{
	//프레임타임을 곱하는 이유는 컴퓨터차이로 인한 속도차이를 없애기위해서
	if (dwDirection)
	{
		float dx = 0, dy = 0;
		if (dwDirection & DIR_FORWARD) //dy -= spd* frame_time;	
			send_move_packet(0);
		if (dwDirection & DIR_BACKWARD) //dy += spd * frame_time; 
			send_move_packet(1);
		if (dwDirection & DIR_RIGHT) //dx += spd * frame_time;	
			send_move_packet(3);
		if (dwDirection & DIR_LEFT) //dx -= spd * frame_time;		
			send_move_packet(2);

		this->dx = dx;
		this->dy = dy;

	}
}

void Player::draw(const HDC& mem1dc)
{
	GameObject::draw(mem1dc, collision_rect);

	if (bitmap)bitmap->draw(mem1dc, pdc, x, y);

	for (int i = 0; i < BLTS; ++i)if (bullets[i]->actived)bullets[i]->draw(mem1dc);
	

}
void Player::update(float fTimeElapsed)
{

	get_BoundingRect(collision_rect); //플레이어의 바운딩박스를 업데이트

	GetCursorPos(&CursorPos);

	//이전위치 업데이트
	px = x;
	py = y;

	//위치 업데이트
	x += dx;
	y += dy;
	
	//이동량 초기화
	dx = 0;
	dy = 0;

	for (int i = 0; i < BLTS; ++i)if (bullets[i]->actived)bullets[i]->update(fTimeElapsed);
}