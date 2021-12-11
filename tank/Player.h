#pragma once
#include "GameBitmap.h"
#include "GameObject.h"
#include "Bullet.h"

#define MAXHP 3
class Player : public GameObject {
	int id;					// player id
	int hp;					// current health point
	float dx, dy;		
	float px, py;			// prior position
	float spd;				// speed 
	GameBitmap** pbmp = NULL;
	GameBitmap* phbmp = NULL;
	int dir = 0;				//0 top 1bottom 2right 3left
	POINT CursorPos;
	RECT collision_rect;	// collision rect
	HDC pdc;
public:
	GameObject** other = NULL; //other object
public:
	Player(HINSTANCE g_hinst, char id, float x, float y);
	~Player();
	int getId();
	int getX();
	int getY();
	int getSpd();
	int gethp();
	int getCursorX();
	int getCursorY();


	void setId(int get_id);
	void setType();
	void setX( int get_x );
	void setY( int get_y );
	void set_aim(float _x, float _y);
	void Player::set_hp(int _hp);
	//�÷��̾��� ������ġ(�̵���)�� ����
	void setPrevPos();

	//�ٸ� ��ü�� �����´�.
	void setObject(GameObject** other);

	void fire();
	//Ű���� ����Ű�� �̵�
	void Move(DWORD dwDirection);
	//�ٸ��÷��̾� �̵��� ���� ������ �Լ�
	void rotate2(float, float);

	virtual void get_BoundingRect(RECT& rect);
	virtual void update(HWND m_hWnd);
	virtual void draw(const HDC& mem1dc);

};