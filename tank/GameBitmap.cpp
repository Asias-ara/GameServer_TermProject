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
	/*BitBlt(mem1dc, 0, 0, width, height, mem2dc,
		dst_rect.left,dst_rect.top, SRCCOPY);*/	

	TransparentBlt(mem1dc, dst_rect.left, dst_rect.top, dst_rect.right, dst_rect.bottom , mem2dc,
		src_rect.left, src_rect.top, src_rect.right, src_rect.bottom, RGB(255,255,255));
	DeleteObject(mem2dc);
}

void GameBitmap::rotate_draw(const HDC& mem1dc, HDC& mem2dc,float x,float y, float ang)
{
	mem2dc = CreateCompatibleDC(mem1dc);

	static HDC temp_dc;
	HBITMAP temp_bmp;

	if (!temp_dc) temp_dc = CreateCompatibleDC(mem1dc);

	temp_bmp = get_rotate_bmp(temp_dc, ang);

	SelectObject(temp_dc, temp_bmp);

	BitBlt(mem2dc, x, y, width, height, temp_dc,
		0, 0, SRCAND);
	DeleteObject(temp_dc);

	int hw = width / 2;
	int hh = height / 2;
	dst_rect.left = x - hw;
	dst_rect.top = y - hh;
	dst_rect.right = hw * 2;
	dst_rect.bottom = hh * 2;
	SelectObject(mem2dc, temp_bmp);
	/*BitBlt(mem1dc, 0, 0, width, height, mem2dc,
		dst_rect.left,dst_rect.top, SRCCOPY);*/
	TransparentBlt(mem1dc, dst_rect.left, dst_rect.top, dst_rect.right, dst_rect.bottom, mem2dc,
		src_rect.left, src_rect.top, src_rect.right, src_rect.bottom, RGB(255, 255, 255));
	DeleteObject(mem2dc);

}

HBITMAP GameBitmap::get_rotate_bmp(HDC& mem1dc, float ang)
{
	HDC srcDC = CreateCompatibleDC(mem1dc);
	HDC dstDC = CreateCompatibleDC(mem1dc);

	
	int x1, y1, x2, y2, x3, y3, minx, miny, maxx, maxy, w, h;

	angle = (float)(ang * PI / 180); //라디안 변환
	float cosine = (float)cos(angle);
	float sine = (float)sin(angle);

	x1 = (int)(height * sine);
	y1 = (int)(height * cosine);
	x2 = (int)(width * cosine + height * sine);
	y2 = (int)(height * cosine - width * sine);
	x3 = (int)(width * cosine);
	y3 = (int)(-width * sine);

	minx = min(0, min(x1, min(x2, x3)));
	miny = min(0, min(y1, min(y2, y3)));
	maxx = max(0, max(x1, max(x2, x3)));
	maxy = max(0, max(y1, max(y2, y3)));

	w = maxx - minx;
	h = maxy - miny;

	HBITMAP res_hdl = CreateCompatibleBitmap(mem1dc, w, h);
	HBITMAP old_bmp = (HBITMAP)SelectObject(srcDC, bitmap);
	HBITMAP res_bmp = (HBITMAP)SelectObject(dstDC, res_hdl);

	HBRUSH back = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH old = (HBRUSH)SelectObject(dstDC, back);
	PatBlt(dstDC, 0, 0, w, h, PATCOPY); //그려질곳에 미리 흰색으로 채움

	DeleteObject(SelectObject(dstDC, old));

	SetGraphicsMode(dstDC, GM_ADVANCED); //회전을 위해 그래픽 확장 모드 변환

	//회전변환 행렬 계산
	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = sine;
	xform.eM21 = -sine;
	xform.eM22 = cosine;
	xform.eDx = (float)width / 2.0f;
	xform.eDy = (float)height / 2.0f;

	//SetWorldTransform(dstDC, &xform);

	BitBlt(dstDC, 0, 0, width, height, srcDC, src_rect.left, src_rect.top, SRCCOPY);
	
	//사용 자원 반환
	SelectObject(srcDC, old_bmp);
	SelectObject(dstDC, res_bmp);
	DeleteObject(srcDC);
	DeleteObject(dstDC);


	return res_hdl;
}

void GameBitmap::set_scale(float w_scale, float h_scale)
{
	width = src_rect.right* w_scale;
	height = src_rect.bottom * h_scale;
}
