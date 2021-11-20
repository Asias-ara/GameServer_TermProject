#pragma once
#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <thread>
#include<unordered_map>
#include "protocol.h"

#define BUFSIZE 2048

using namespace std;

//플레이어 가로,세로 추후 수정필요
const int player_width = 10;
const int player_height = 10;