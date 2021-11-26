#pragma once
#include"header.h"
class Wall
{
public:
	Wall() = default;
	Wall(float x, float y, float w, float h) :m_pos_x(x), m_pos_y(y),m_width(w),m_height(h) {
		get_BoundingRect(); 
	};
	~Wall() {};
public:
	float m_pos_x, m_pos_y;
	float m_width, m_height;
	RECT collision_rect;
	void get_BoundingRect();

};

