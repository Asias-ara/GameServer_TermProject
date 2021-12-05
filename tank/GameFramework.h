#pragma once
#include <chrono>
#include "Player.h"
#include "Wall.h"
#include "UI.h"
#include "BackGround.h"
class GameFramework {
public:
	GameFramework(void);
	~GameFramework(void);

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();

	void SetActive(bool bActive) { m_bActive = bActive; }

	chrono::steady_clock::time_point start_t;


private:
	HINSTANCE					m_hInstance = NULL;
	HWND						m_hWnd = NULL;

	bool						m_bActive = true;

	HDC							hDCFrameBuffer = NULL;
	HBITMAP						m_hBitmapFrameBuffer = NULL;

	
	BG* m_pBG = NULL;
	UI* m_pUI = NULL;
	Player* m_pPlayer = NULL; //멀티 플레이시 이중포인터로 바꿔주어야함
	Player*	m_pOther[2];		// 다른 플레이어들
	BulletObject* m_bullets[15];	// 총알은 플레이어당 5개 3*5 = 15;
	GameObject** objects = NULL;

	RECT obj_map[NOBJECTS] = {
		0,0,800,80,
		0,720,800,80,
		0,80,80,640,
		720,80,800,640,
		345,106,110,24,
		345,668,110,24,
		105,345,24,110,
		668,345,24,110,
		220,220,99,35,
		482,220,99,35,
		220,545,99,35,
		482,545,99,35,
		220,255,35,63,
		545,255,35,63,
		220,482,35,63,
		545,482,35,63
	};

public:

	void BuildFrameBuffer();
	void ClearFrameBuffer(DWORD dwColor);
	void PresentFrameBuffer();

	void BuildObjects();
	void ReleaseObjects();
	void ProcessInput();

	//마우스처리 함수
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	//키보드처리 함수
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	_TCHAR						szFrameRate[50];
};