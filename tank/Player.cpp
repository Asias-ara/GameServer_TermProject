#include "stdafx.h"
#include "Player.h"
#include "Wall.h"
#include "Network.h"

Player::Player(HINSTANCE g_hinst, char id, float x, float y) 
	:GameObject(g_hinst,x,y)
{
	this->id = id;
	dx = 0;
	dy = 0;
	spd = 100;
	hp = 3;
	dir = 0;
	GetCursorPos(&CursorPos);

	pbmp = new GameBitmap * [4];
	pbmp[0] = new GameBitmap("img/tank0.bmp", g_hinst);
	pbmp[1] = new GameBitmap("img/tank1.bmp", g_hinst);
	pbmp[2] = new GameBitmap("img/tank2.bmp", g_hinst);
	pbmp[3] = new GameBitmap("img/tank3.bmp", g_hinst);
	phbmp = new GameBitmap * [4];
	phbmp[0] = new GameBitmap("img/tank_head0.bmp", g_hinst);
	phbmp[1] = new GameBitmap("img/tank_head1.bmp", g_hinst);
	phbmp[2] = new GameBitmap("img/tank_head2.bmp", g_hinst);
	phbmp[3] = new GameBitmap("img/tank_head3.bmp", g_hinst);
	for (int i = 0; i < 4; ++i)
	{
		pbmp[i]->set_scale(0.2f,0.2f);
		phbmp[i]->set_scale(0.2f,0.2f);
	}

	get_BoundingRect(collision_rect);

}

Player::~Player()
{
	if (other) other = NULL;
}

int Player::getId()
{
	return id;
}

int Player::gethp()
{
	return hp;
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

void Player::set_hp(int _hp)
{
	hp = _hp;
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
	
}

void Player::Move(DWORD dwDirection)
{

	if (dwDirection)
	{
		if (dwDirection & DIR_FORWARD) 
		{
			dir = 0;
			send_move_packet(0);
		}	
		if (dwDirection & DIR_BACKWARD)
		{
			dir = 1;
			send_move_packet(1);
		}
		if (dwDirection & DIR_RIGHT) 
		{
			dir = 2;
			send_move_packet(3);
		}
		if (dwDirection & DIR_LEFT)
		{
			dir = 3;
			send_move_packet(2); 
		}


	}
}

void Player::rotate(HWND m_hWnd)
{
	CursorPos.x = get_Aim_x(id);
	CursorPos.y = get_Aim_y(id);
	if (id == get_my_id()) {
		GetCursorPos(&CursorPos);
		ScreenToClient(m_hWnd, &CursorPos);
	}
	float dx = 0;
	float dy = 0;

	POINT start;
	start.x = x; start.y = y;

	Normalize(start, CursorPos, dx, dy); 

	if (abs(dx) > abs(dy))
	{
		if (dx > 0) hdir = 2;
		else if (dx < 0) hdir = 3;
	}
	else if (abs(dx) < abs(dy))
	{
		if (dy > 0) hdir = 1;
		else if (dy < 0) hdir = 0;
	}
}

int Player::getCursorX()
{
	return CursorPos.x;
}
int Player::getCursorY()
{
	return CursorPos.y;
}

void Player::draw(const HDC& mem1dc)
{
	GameObject::draw(mem1dc, collision_rect);
	float xoffset = 0;
	float yoffset = 0;
	if (pbmp[dir])pbmp[dir]->draw(mem1dc, pdc, x, y);
	if (dir != hdir)
	{
		if (dir == 0)
		{
			if (hdir == 1)
			{
				yoffset = 10;
			}
			else if (hdir == 2)
			{
				xoffset = 10;
				yoffset = 5;

			}
			else if (hdir == 3)
			{
				xoffset = -10;
				yoffset = 5;
			}

		}
		else if (dir == 1)
		{
			if (hdir == 0)
			{
				yoffset = -15;
			}
			else if (hdir == 2)
			{
				xoffset = 10;
				yoffset = -5;

			}
			else if (hdir == 3)
			{
				xoffset = -10;
				yoffset = -5;
			}
		}
		else if (dir == 2)
		{
			if (hdir == 0)
			{
				xoffset = -5;
				yoffset = -5;
			}
			else if (hdir == 1)
			{
				xoffset = -5;
				yoffset = 5;

			}
			else if (hdir == 3)
			{
				xoffset = -15;

			}
		}else if (dir == 3)
		{
			if (hdir == 0)
			{
				xoffset = 5;
				yoffset = -5;
			}
			else if (hdir == 1)
			{
				xoffset = 5;
				yoffset = 5;

			}
			else if (hdir == 2)
			{
				xoffset = 15;

			}
		}
	}
	if (phbmp[hdir])phbmp[hdir]->draw(mem1dc, pdc, x+xoffset, y+yoffset);
	

}
void Player::get_BoundingRect(RECT& rect)
{
	//원래이미지보다 한픽셀 크게 그리기
	int hw = pbmp[dir]->getWidth() / 2 + 1;
	int hh = pbmp[dir]->getHeight() / 2 + 1;
	rect.left = x - hw;
	rect.top = y - hh;
	rect.right = x + hw;
	rect.bottom = y + hh;
}
void Player::rotate2(float nx ,float ny) 
{
	float dx = x - nx;
	float dy = y - ny;
	if ((int)x != (int)nx)
	{
		if (dx > 0) dir = 3;
		else if (dx < 0) dir = 2;
	}
	else if ((int)y != (int)ny)
	{
		if (dy > 0) dir = 0;
		else if (dy < 0) dir = 1;
	}
}
void Player::update(HWND m_hWnd)
{

	get_BoundingRect(collision_rect); //플레이어의 바운딩박스를 업데이트



	//이전위치 업데이트
	px = x;
	py = y;

	//위치 업데이트
	x += dx;
	y += dy;
	
	//이동량 초기화
	dx = 0;
	dy = 0;
	
}
