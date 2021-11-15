#pragma once


//소켓 헤더파일
#include <WS2tcpip.h>
// Windows 헤더 파일:
#include <windows.h>
//C++
#include <iostream>
//C 런타임 헤더파일
#include <tchar.h>
#include <math.h>
#include <stdlib.h>



using namespace std;


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


void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[])
{
	int nLen = (int)strlen(pstrSrc) + 1;
	mbstowcs(pwstrDest, pstrSrc, nLen);
}