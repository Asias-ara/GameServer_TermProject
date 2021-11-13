#pragma once

class GameBitmap {
protected:
	HBITMAP bitmap;
public:
	GameBitmap(char* filename, HINSTANCE g_hinst); // 
	HBITMAP load(char* filename, HINSTANCE g_hinst); // 비트맵이미지 로드함수
	void draw(const HDC& mem1dc, HDC& mem2dc, float x, float y);

	int getHeight() { return height; }
	int getWidth() { return width; }
protected:
	RECT src_rect;//필요가 있나?
	RECT dst_rect;
	int width;
	int height;

};