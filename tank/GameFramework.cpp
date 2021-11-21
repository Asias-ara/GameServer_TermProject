#include "stdafx.h"
#include "GameFramework.h"
#include "Network.h"

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
	//객체 생성
	
	
	objects = new GameObject * [NOBJECTS];
	for (int i = 0; i < NOBJECTS; ++i) {
		Wall* w= new Wall(m_hInstance, obj_map[i].x, obj_map[i].y);
		objects[i] = w;
	}
	

	//플레이어의 정보를 서버로부터 받아서 생성
	m_pPlayer = new Player(m_hInstance, get_my_id(), 150, 150);
	m_pPlayer->setId(get_my_id());
	int i = 1;
	for (auto& other_player : m_pOther) {
		if (i == get_my_id()) ++i;
		other_player = new Player(m_hInstance, i, 100*i, 100*i);
		other_player->setId(i);
		//other_player->setObject(objects);
		++i;
	}
	m_pPlayer->setObject(objects);
}

void GameFramework::ReleaseObjects()
{
	if (m_pPlayer) delete m_pPlayer;
	for(int i =0; i < NOBJECTS; ++i) if (objects[i]) delete objects[i];
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
	
	

	if (dwDirection) m_pPlayer->Move(dwDirection, GameTimer.GetTimeElapsed());
	
	m_pPlayer->update(GameTimer.GetTimeElapsed());

	RECT o_crect,p_crect; //물체의 바운딩박스 플레이어의 바운딩박스
	bool collided = false;

	m_pPlayer->get_BoundingRect(p_crect);
	for (int i = 0; i < NOBJECTS; ++i) //멀티 플레이시 이중for문으로 비교
	{
		objects[i]->get_BoundingRect(o_crect);
		if (collide(o_crect, p_crect)) {
			m_pPlayer->setPrevPos();//이전위치로 재설정
		}
	}
}

void GameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONDOWN: {
		send_attack_packet();
		//m_pPlayer->fire();
		break;
	}
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

	// 위치 수정
	m_pPlayer->setX(get_MyPosition_x()); 
	m_pPlayer->setY(get_MyPosition_y());
	// 여기서 플레이어 총구 설정

	for (auto& other: m_pOther) {
		other->setX(get_Position_x(other->getId()));
		other->setY(get_Position_y(other->getId()));
		// 다른 플레이어 총구 설정
	}

	float fTimeElapsed = GameTimer.GetTimeElapsed();
	//애니메이션

	ClearFrameBuffer(RGB(255, 255, 255));

	//렌더링
	for(int i =0; i<NOBJECTS; ++i)
		((Wall*)objects[i])->draw(hDCFrameBuffer);
	for (auto& other : m_pOther) {
		other->draw(hDCFrameBuffer);
		other->update(GameTimer.GetTimeElapsed());
	}

	m_pPlayer->draw(hDCFrameBuffer);


	PresentFrameBuffer();

	GameTimer.GetFrameRate(szFrameRate + 12, 37);
	::SetWindowText(m_hWnd, szFrameRate);

	m_pPlayer->send_cursor();
}