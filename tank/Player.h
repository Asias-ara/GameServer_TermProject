#pragma once
#include "GameBitmap.h"
#include "GameObject.h"
#include "Bullet.h"

#define BLTS 5
#define MAXHP 3
class Player : public GameObject {
	int id;				// player id
	int hp;					// current health point
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
	int getId();
	int getX();
	int getY();
	int getSpd();
	int gethp();

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
	void Move(DWORD dwDirection, float frame_time);


	virtual void update(HWND m_hWnd, float fTimeElapsed);
	virtual void draw(const HDC& mem1dc);

	void send_cursor(HWND m_hWnd);
};