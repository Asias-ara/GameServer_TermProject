#pragma once
#include "GameBitmap.h"
#include "GameObject.h"
#include "Player.h"

#define UIBMP 2



class UI {
	int x, y;
	float offset = 0; //하트사이의 간격을 조절하는 값입니다.
	Player* player = NULL;	//클라이언트의 플레이어객체를 가리킨다.
	GameBitmap** bitmap = NULL;
	HDC uidc;
public:
	UI(HINSTANCE g_hinst, Player* p);

	void get_player(Player* p);

	virtual void update(Player* p);
	virtual void draw(const HDC& mem1dc);
};
