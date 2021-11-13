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

	Player* m_pPlayer = NULL;
	Wall* w_objects = NULL;

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