#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(HINSTANCE g_hinst, float x, float y)
{
	this->x = x;
	this->y = y;
	
}

GameObject::GameObject(HINSTANCE g_hinst, char* filename, float x, float y)
{
	this->x = x;
	this->y = y;
	this->bitmap = new GameBitmap(filename, g_hinst);

}

void GameObject::get_BoundingRect(RECT& rect)
{
	//원래이미지보다 한픽셀 크게 그리기
	int hw = bitmap->getWidth() / 2 + 1;
	int hh = bitmap->getHeight() / 2 + 1;
	rect.left = x - hw;
	rect.top = y - hh;
	rect.right = x + hw;
	rect.bottom = y + hh;
}

void GameObject::draw_BoundingRect(const HDC hdc, const RECT rect)
{
	HPEN hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN open = (HPEN)SelectObject(hdc, hpen);
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	SelectObject(hdc, open);
	DeleteObject(hpen);
}
void GameObject::draw(const HDC& hdc, const RECT rect)
{
#ifdef __DEBUG
	draw_BoundingRect(hdc, rect);
#endif
}
