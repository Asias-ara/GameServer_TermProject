#pragma once
#include "GameBitmap.h"
#include "GameObject.h"
#include "Bullet.h"

#define BLTS 5

class Player : public GameObject {
	char id;				// player id
	int hp;					// health point
	float dx, dy;		
	float px, py;			// prior position
	float spd;				// speed 
	POINT CursorPos;
	RECT collision_rect;	// collision rect
	HDC pdc;
public:
	GameObject** other = NULL; //other object
	BulletObject* bullets[BLTS];
public:
	Player(HINSTANCE g_hinst, char id, float x, float y);
	~Player();
	char getId();
	int getX();
	int getY();
	int getSpd();

	void setId();
	void setType();
	void setX();
	void setY();
	//플레이어의 이전위치(이동전)로 설정
	void setPrevPos();

	//다른 객체를 가져온다.
	void setObject(GameObject** other);

	void fire();
	//키보드 방향키로 이동
	void Move(DWORD dwDirection, float frame_time);


	virtual void update(float fTimeElapsed);
	virtual void draw(const HDC& mem1dc);
};