#pragma once

#include "protocol.h"

const int BUFSIZE = 2048;


void err_quit(const char* msg);

void err_display(const char* msg);

int netInit();

int netclose();

void send_move_packet(int direction);


void do_send(int num_bytes, void* mess);

void do_recv();

// void return_otherPlayer(CPlayer** m_otherPlayer, ID3D12Device* m_pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);