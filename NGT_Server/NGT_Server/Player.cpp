#include "Player.h"

Player::Player():m_aim_x(0.f),m_aim_y(0.f),m_spd(100),m_pos_x(0.f),m_pos_y(0.f),m_id(0)
{
	
}


Player::Player(SOCKET& s, int id) : m_c_socket(s),m_spd(100), m_id(id), m_aim_x(0.f), m_aim_y(0.f), m_pos_x(0.f), m_pos_y(0.f)
{
	
	
}

Player::~Player()
{
	//closesocket(m_c_socket);
}

void Player::get_BoundingRect()
{
	int hw = player_width / 2 + 1;
	int hh = player_height / 2 + 1;
	collision_rect.left = m_pos_x - hw;
	collision_rect.top = m_pos_y - hh;
	collision_rect.right = m_pos_x + hw;
	collision_rect.bottom = m_pos_y + hh;
}

void Player::update(float fTimeElapsed)
{
	get_BoundingRect();

	m_px = m_pos_x;
	m_py = m_pos_y;

	//위치 업데이트
	m_pos_x += m_dx;
	m_pos_y += m_dy;

	//이동량 초기화
	m_dx = 0;
	m_dy = 0;
}
