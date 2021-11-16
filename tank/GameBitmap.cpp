#include "stdafx.h"
#include "GameBitmap.h"

GameBitmap::GameBitmap(char* filename,HINSTANCE g_hinst)
{
	bitmap = load(filename, g_hinst);
	BITMAP bmp;
	GetObject(bitmap, sizeof(BITMAP), &bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;
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
	int hw = width / 2;
	int hh = height / 2;
	dst_rect.left = x - hw;
	dst_rect.top = y - hh;
	dst_rect.right = width;
	dst_rect.bottom = height;
	mem2dc = CreateCompatibleDC(mem1dc);
	SelectObject(mem2dc, bitmap);
	/*BitBlt(mem1dc, 0, 0, width, height, mem2dc,
		dst_rect.left,dst_rect.top,dst_rect.right,dst_rect.bottom, SRCCOPY);*/	
	TransparentBlt(mem1dc, dst_rect.left, dst_rect.top, dst_rect.right, dst_rect.bottom , mem2dc,
		0, 0, width, height, RGB(255,255,255));
	DeleteObject(mem2dc);
}