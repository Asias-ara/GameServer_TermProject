#include "stdafx.h"
#include "GameBitmap.h"

GameBitmap::GameBitmap(char* filename,HINSTANCE g_hinst)
{
	bitmap = load(filename, g_hinst);
	BITMAP bmp;
	GetObject(bitmap, sizeof(BITMAP), &bmp);
	src_rect = RECT{ 0, 0, bmp.bmWidth, bmp.bmHeight };
	width = bmp.bmWidth * scale;
	height = bmp.bmHeight * scale;
}

HBITMAP GameBitmap::load(char * filename, HINSTANCE g_hinst)
{
	TCHAR str[20] = { 0 };
	CharToWChar(filename, str);
	HBITMAP bmp = (HBITMAP)LoadImage(g_hinst,str, 
IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	return bmp;
}

void GameBitmap::draw(const HDC& mem1dc, HDC& mem2dc,float x, float y)
{
	int hw = width / 2 ;
	int hh = height / 2 ;
	dst_rect.left = x - hw;
	dst_rect.top = y - hh;
	dst_rect.right = hw * 2;
	dst_rect.bottom = hh * 2;
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, bitmap);	

	TransparentBlt(mem1dc, dst_rect.left, dst_rect.top, dst_rect.right, dst_rect.bottom , mem2dc,
		src_rect.left, src_rect.top, src_rect.right, src_rect.bottom, RGB(255,255,255));
	DeleteObject(mem2dc);
}

void GameBitmap::set_scale(float w_scale, float h_scale)
{
	width = src_rect.right* w_scale;
	height = src_rect.bottom * h_scale;
}
