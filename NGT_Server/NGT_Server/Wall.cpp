#include "Wall.h"

void Wall::get_BoundingRect()
{
	//int hw = wall_width / 2 + 1;
	//int hh = wall_height / 2 + 1;
	collision_rect.left = m_pos_x;
	collision_rect.top = m_pos_y;
	collision_rect.right = m_pos_x + m_width;
	collision_rect.bottom = m_pos_y + m_height;
}
