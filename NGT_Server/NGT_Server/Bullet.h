#pragma once

class BulletObject{
	int		id;		// 누가 쐈는가?		
	float	x, y;	// 총알의 위치
	float	spd;	// 총알의 속도
	float	dx, dy;	// 총알의 방향
	bool	actived;	// 존재하는가 아닌가
public:
	BulletObject();

	void setDir(float _dx, float _dy);
	void setPos(float _x, float _y);
	void setActive();

	float getPos_x();
	float getPos_y();
	bool getActive();

	void update(float fTimeElapsed);
};
