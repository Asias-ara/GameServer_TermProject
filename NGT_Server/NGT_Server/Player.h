#pragma once
#include "header.h"
class Player
{
public:
	Player();
	Player(SOCKET& s, int id);
	~Player();

public:
	int m_id;
	float m_aim_x, m_aim_y;
	float m_pos_x, m_pos_y;
	float m_dx, m_dy;
	float m_px, m_py;
	float m_spd;
	int m_hp = 3;
	SOCKET m_c_socket;
	int m_prev_size;
	char m_buf[BUFSIZE];
	RECT collision_rect;	// collision rect
public:
	void get_BoundingRect();
	void update(float fTimeElapsed);
	//플레이어의 이전위치(이동전)로 설정
	void setPrevPos();

};

