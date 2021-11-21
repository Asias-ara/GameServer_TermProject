#include "Player.h"

Player::Player():m_aim_x(0.f),m_aim_y(0.f),m_pos_x(0.f),m_pos_y(0.f),m_id(0)
{
	m_prev_size = 0;
}


Player::Player(SOCKET& s, int id) : m_c_socket(s), m_id(id), m_aim_x(0.f), m_aim_y(0.f), m_pos_x(0.f), m_pos_y(0.f)
{
	m_prev_size = 0;
	int hw = player_width / 2 + 1;
	int hh = player_height / 2 + 1;
	collision_rect.left = m_pos_x - hw;
	collision_rect.top = m_pos_y - hh;
	collision_rect.right = m_pos_x + hw;
	collision_rect.bottom = m_pos_y + hh;
}

Player::~Player()
{
	//closesocket(m_c_socket);
}
