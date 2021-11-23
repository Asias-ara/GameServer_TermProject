#pragma once
#include "GameBitmap.h"
#include "GameObject.h"
#include "Player.h"

#define UIBMP 2



class UI {
	int x, y;
	float offset = 0; //��Ʈ������ ������ �����ϴ� ���Դϴ�.
	Player* player = NULL;	//Ŭ���̾�Ʈ�� �÷��̾ü�� ����Ų��.
	GameBitmap** bitmap = NULL;
	HDC uidc;
public:
	UI(HINSTANCE g_hinst, Player* p);

	void get_player(Player* p);

	virtual void update(Player* p);
	virtual void draw(const HDC& mem1dc);
};
