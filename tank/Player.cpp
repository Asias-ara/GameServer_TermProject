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

int Player::getId()
{
	return id;
}

void Player::setId(int get_id)
{
	id = get_id;
}

void Player::setX(int get_x)
{
	x = get_x;
}

void Player::setY(int get_y)
{
	y = get_y;
}

void Player::set_aim(float _x, float _y)
{
	CursorPos.x = _x;
	CursorPos.y = _y;
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
	//������Ÿ���� ���ϴ� ������ ��ǻ�����̷� ���� �ӵ����̸� ���ֱ����ؼ�
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
void Player::update(HWND m_hWnd, float fTimeElapsed)
{

	get_BoundingRect(collision_rect); //�÷��̾��� �ٿ���ڽ��� ������Ʈ

	

	//������ġ ������Ʈ
	px = x;
	py = y;

	//��ġ ������Ʈ
	x += dx;
	y += dy;
	
	//�̵��� �ʱ�ȭ
	dx = 0;
	dy = 0;
	
	for (int i = 0; i < BLTS; ++i)if (bullets[i]->actived)bullets[i]->update(fTimeElapsed);
}

void Player::send_cursor(HWND m_hWnd)
{
	GetCursorPos(&CursorPos);

	ScreenToClient(m_hWnd, &CursorPos);
	send_aim_packet(CursorPos.x, CursorPos.y);
}