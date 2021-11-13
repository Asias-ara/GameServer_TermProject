#pragma once
#include "GameBitmap.h"
#include "GameObject.h"

class Player : public GameObject {
	char id;				// player id
	int hp;					// health point
	float dx, dy;		
	float px, py;			// prior position
	float spd;				// speed 
	RECT collision_rect;	// collision rect
	HDC pdc;
public:
	GameObject* other = NULL; //other object
public:
	Player(HINSTANCE g_hinst, char id, float x, float y);

	char getId();
	int getX();
	int getY();
	int getSpd();

	void setId();
	void setType();
	void setX();
	void setY();
	//다른 객체를 가져온다.
	void setObject(GameObject* other);

	void fire();
	void Move(DWORD dwDirection, float frame_time);


	virtual void update(float fTimeElapsed);
	virtual void draw(const HDC& mem1dc);
};