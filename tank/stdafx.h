#pragma once


//소켓 헤더파일
#include <WS2tcpip.h>
// Windows 헤더 파일:
#include <windows.h>
//C++
#include <iostream>
#include <cmath>
//C 런타임 헤더파일
#include <tchar.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

// 콘솔창
#ifdef _DEBUG

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#endif


#pragma comment(lib, "ws2_32")
#pragma comment(lib, "winmm.lib") 
#pragma comment (lib, "Msimg32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define CLIENT_WIDTH			800
#define CLIENT_HEIGHT			800
#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20

//#define __DEBUG

#define PI 3.141592654

#define NOBJECTS 16 //벽40  + 장애물 2 + 다른 플레이어

extern bool collide(const RECT& rect1, const RECT& rect2);
extern void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[]);
extern void Normalize(const POINT& start, const POINT& end, float& dx, float& dy);