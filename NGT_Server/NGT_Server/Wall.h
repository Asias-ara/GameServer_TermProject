#pragma once
#include"header.h"
class Wall
{
public:
	Wall() = default;
	Wall(float x, float y) :m_pos_x(x), m_pos_y(y) { 
		get_BoundingRect(); 
	};
	~Wall() {};
public:
	float m_pos_x, m_pos_y;
	RECT collision_rect;
	void get_BoundingRect();

};

