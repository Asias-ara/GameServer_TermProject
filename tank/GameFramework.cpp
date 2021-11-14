#include "stdafx.h"
#include "GameFramework.h"

GameFramework::GameFramework()
{
	_tcscpy_s(szFrameRate, _T("Tank Hero ("));
}

GameFramework::~GameFramework()
{
}

bool GameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	::srand(timeGetTime());

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	BuildFrameBuffer();

	BuildObjects();

	return(true);
}

void GameFramework::BuildFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);

	hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, (rcClient.right - rcClient.left) + 1, (rcClient.bottom - rcClient.top) + 1);
	::SelectObject(hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
	::SetBkMode(hDCFrameBuffer, TRANSPARENT);
}

void GameFramework::ClearFrameBuffer(DWORD dwColor)
{
	HBRUSH hBrush = ::CreateSolidBrush(dwColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDCFrameBuffer, hBrush);
	::Rectangle(hDCFrameBuffer, 0,0,CLIENT_WIDTH,CLIENT_HEIGHT);
	::SelectObject(hDCFrameBuffer, hOldBrush);
	::DeleteObject(hBrush);
}

void GameFramework::PresentFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);
	::BitBlt(hDC, 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT,hDCFrameBuffer,0,0, SRCCOPY);
	::ReleaseDC(m_hWnd, hDC);
}

void GameFramework::BuildObjects()
{
	//��ü ����
	
	w_objects = new Wall(m_hInstance, 50, 50);
	//�÷��̾��� ������ �����κ��� �޾Ƽ� ����
	m_pPlayer = new Player(m_hInstance, 1, 150, 150);
	m_pPlayer->setObject(w_objects);

}
void GameFramework::ReleaseObjects()
{
	if (m_pPlayer) delete m_pPlayer;
	if (w_objects) delete w_objects;
}

void GameFramework::ProcessInput()
{
	static UCHAR pKeyBuffer[256];
	DWORD dwDirection = 0;
	if (GetKeyboardState(pKeyBuffer))
	{
		if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
		if (pKeyBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;
	}
	
	
	POINT ptCursorPos;
	//���콺 Ŀ����ġ�� �÷��̾�� �����ϴºκ�

	if (dwDirection) m_pPlayer->Move(dwDirection, GameTimer.GetTimeElapsed());
	
	m_pPlayer->update(GameTimer.GetTimeElapsed());
}

void GameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void GameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_CONTROL:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT GameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			GameTimer.Stop();
		else
			GameTimer.Start();
		break;
	}
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void GameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (hDCFrameBuffer) ::DeleteDC(hDCFrameBuffer);

	if (m_hWnd) DestroyWindow(m_hWnd);
}

void GameFramework::FrameAdvance()
{
	if (!m_bActive) return;

	GameTimer.Tick(0.0f);

	ProcessInput();

	float fTimeElapsed = GameTimer.GetTimeElapsed();
	//�ִϸ��̼�

	ClearFrameBuffer(RGB(255, 255, 255));

	//������
	w_objects->draw(hDCFrameBuffer);
	
	m_pPlayer->draw(hDCFrameBuffer);


	PresentFrameBuffer();

	GameTimer.GetFrameRate(szFrameRate + 12, 37);
	::SetWindowText(m_hWnd, szFrameRate);
}