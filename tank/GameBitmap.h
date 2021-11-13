#pragma once

class GameBitmap {
protected:
	HBITMAP bitmap;
public:
	GameBitmap(char* filename, HINSTANCE g_hinst); // 
	HBITMAP load(char* filename, HINSTANCE g_hinst); // ��Ʈ���̹��� �ε��Լ�
	void draw(const HDC& mem1dc, HDC& mem2dc, float x, float y);

	int getHeight() { return height; }
	int getWidth() { return width; }
protected:
	RECT src_rect;//�ʿ䰡 �ֳ�?
	RECT dst_rect;
	int width;
	int height;

};