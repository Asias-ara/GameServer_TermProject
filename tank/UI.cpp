#include "stdafx.h"
#include "UI.h"

UI::UI(HINSTANCE g_hinst,Player *p)
{
	x = 700;
	y = 50;
	offset = 1.2f;
	get_player(p);
	bitmap = new GameBitmap * [UIBMP];
	bitmap[0] = new GameBitmap("img/hp_empty.bmp", g_hinst); //bmp;
	bitmap[1] = new GameBitmap("img/hp_full.bmp", g_hinst); //bmp2;
}

void UI::get_player(Player* p)
{
	player = p;
}

void UI::update(Player* p)
{
	get_player(p);
}

void UI::draw(const HDC& mem1dc)
{
	for (int i = 0; i < MAXHP; ++i)
	{
		if (bitmap[0])
			bitmap[0]->draw(mem1dc, uidc, x + i * bitmap[0]->getWidth()*offset, y);	
	}
	for (int i = 0; i < player->gethp(); ++i)
	{
		if (bitmap[1])
			bitmap[1]->draw(mem1dc, uidc, x + i * bitmap[1]->getWidth()*offset, y);
	}
}
