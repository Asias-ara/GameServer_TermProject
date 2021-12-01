#include "stdafx.h"
#include "GameFramework.h"
#include "Network.h"
#include<chrono>
GameFramework::GameFramework()
{
	_tcscpy_s(szFrameRate, _T("Tank Hero ("));
	start_t = chrono::high_resolution_clock::now();
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
		Wall* w= new Wall(m_hInstance, obj_map[i].left, obj_map[i].top, obj_map[i].right, obj_map[i].bottom);
		//w->ImagetoScreen();
		objects[i] = w;
	}
	
	for (auto& bullet : m_bullets)
		bullet = new BulletObject(m_hInstance, -1.0f, -1.0f);

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
	m_pUI = new UI(m_hInstance, m_pPlayer);
	m_pBG = new BG(m_hInstance,CLIENT_WIDTH/2,CLIENT_HEIGHT/2);
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

	auto t = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_t).count();
	if (GetKeyboardState(pKeyBuffer) && t >= 10)

	{
		if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
		if (pKeyBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;

		start_t = chrono::high_resolution_clock::now();
	}
	
	

	if (dwDirection) m_pPlayer->Move(dwDirection, GameTimer.GetTimeElapsed());
	
	m_pPlayer->update(m_hWnd,GameTimer.GetTimeElapsed());

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
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);
		cout << "Mouse X :" << pt.x << " Mouse Y :" << pt.y << endl;
		break;
	case WM_LBUTTONDOWN: 
		
		m_pPlayer->rotate(hWnd);
		send_attack_packet( m_pPlayer->getCursorX(), m_pPlayer->getCursorY());
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

	// 위치 수정
	m_pPlayer->rotate2(get_MyPosition_x(), get_MyPosition_y());
	m_pPlayer->setX(get_MyPosition_x()); 
	m_pPlayer->setY(get_MyPosition_y());
	m_pPlayer->set_hp(get_my_hp());
	// 여기서 플레이어 총구 설정
	//m_pPlayer->set_aim(get_Aim_x(get_my_id()), get_Aim_y(get_my_id()));

	for (auto& other: m_pOther) {
		other->rotate2(get_Position_x(other->getId()), get_Position_y(other->getId()));
		other->setX(get_Position_x(other->getId()));
		other->setY(get_Position_y(other->getId()));
		// 다른 플레이어 총구 설정
		//other->set_aim(get_Aim_x(other->getId()),get_Aim_y(other->getId()));
	}

	//UI가 가지고있는 플레이어에 대한 정보를 업데이트 시켜줌
	m_pUI->update(m_pPlayer);

	float fTimeElapsed = GameTimer.GetTimeElapsed();
	//애니메이션

	ClearFrameBuffer(RGB(255, 255, 255));
	
	//렌더링
	m_pBG->draw(hDCFrameBuffer);
	for(int i =0; i<NOBJECTS; ++i)
		((Wall*)objects[i])->draw(hDCFrameBuffer);
	// 총알 렌더링
	for (int i = 0; i < 15; ++i) {
		m_bullets[i]->setActive(get_bullet_active(i));
		if (m_bullets[i]->getActive()) {	// 이미 움직이는 중이다
			m_bullets[i]->setPos(get_bullet_x(i), get_bullet_y(i));
			m_bullets[i]->draw(hDCFrameBuffer);
		}
	}

	for (auto& other : m_pOther) {
		if (get_activate(other->getId()) == false) continue;
		other->draw(hDCFrameBuffer);
		other->update(m_hWnd,GameTimer.GetTimeElapsed());
	}
	if (get_activate(m_pPlayer->getId())) {
		m_pPlayer->draw(hDCFrameBuffer);
	}
	m_pUI->draw(hDCFrameBuffer);

	PresentFrameBuffer();

	GameTimer.GetFrameRate(szFrameRate + 12, 37);
	::SetWindowText(m_hWnd, szFrameRate);

	m_pPlayer->send_cursor(m_hWnd);
}