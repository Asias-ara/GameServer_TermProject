#pragma once
#include "Timer.h"
#include "Player.h"
#include "Wall.h"
class GameFramework {
public:
	GameFramework(void);
	~GameFramework(void);

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();

	void SetActive(bool bActive) { m_bActive = bActive; }

private:
	HINSTANCE					m_hInstance = NULL;
	HWND						m_hWnd = NULL;

	bool						m_bActive = true;

	GameTimer					GameTimer;

	HDC							hDCFrameBuffer = NULL;
	HBITMAP						m_hBitmapFrameBuffer = NULL;

	
	Player* m_pPlayer = NULL; //멀티 플레이시 이중포인터로 바꿔주어야함
	Player*	m_pOther[2];		// 다른 플레이어들
	GameObject** objects = NULL;

	POINT obj_map[NOBJECTS] = {
		25,25,
		75,25,
		125,25,
		175,25,
		225,25,
		275,25,
		325,25,
		375,25,
		425,25,
		475,25,
		525,25,
		575,25,
		625,25,//TOP 
		25,435,
		75,435,
		125,435,
		175,435,
		225,435,
		275,435,
		325,435,
		375,435,
		425,435,
		475,435,
		525,435,
		575,435,
		625,435,//BOTTOM 
		25,75,
		25,125,
		25,175,
		25,225,
		25,275,
		25,325,
		25,375, //LEFT
		625,75,
		625,125,
		625,175,
		625,225,
		625,275,
		625,325,
		625,375, //RIGHT
		325,125,
		325,325,
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