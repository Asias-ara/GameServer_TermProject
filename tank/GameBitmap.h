#pragma once



class GameBitmap {
public:
	GameBitmap(char* filename, HINSTANCE g_hinst); // 
	HBITMAP load(char* filename, HINSTANCE g_hinst); // 비트맵이미지 로드함수
	void draw(const HDC& mem1dc, HDC& mem2dc, float x, float y);
	void rotate_draw(const HDC& mem1dc, HDC& mem2dc, float x, float y, float ang);

	HBITMAP get_rotate_bmp(HDC& mem1dc, float ang);

	int getHeight() { return height; }
	int getWidth() { return width; }
	void set_scale(float w_sclae, float h_scale);

protected:
	RECT src_rect;//필요가 있나?
	RECT dst_rect;
	int width;
	int height;

	float scale = 1.0f;
	float angle = 0.0f;

	HBITMAP bitmap;

};