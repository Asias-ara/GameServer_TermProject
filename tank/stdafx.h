#pragma once


//���� �������
#include <WS2tcpip.h>
// Windows ��� ����:
#include <windows.h>
//C++
#include <iostream>
#include <cmath>
//C ��Ÿ�� �������
#include <tchar.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

// �ܼ�â
#ifdef _DEBUG

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#endif


#pragma comment(lib, "ws2_32")
#pragma comment(lib, "winmm.lib") 
#pragma comment (lib, "Msimg32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define CLIENT_WIDTH			640
#define CLIENT_HEIGHT			480
#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20

#define __DEBUG

#define NOBJECTS 42 //��40  + ��ֹ� 2 + �ٸ� �÷��̾�

extern bool collide(const RECT& rect1, const RECT& rect2);
extern void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[]);
extern void Normalize(const POINT& start, const POINT& end, float& dx, float& dy);