#pragma once

#include "protocol.h"
// #include "Player.h"
#include <array>

const int BUFSIZE = 2048;

void err_quit(const char* msg);

void err_display(const char* msg);

int netInit();

int netclose();

void send_attack_packet(float aim_x, float aim_y);

void send_aim_packet(float x, float y);

void send_move_packet(int direction);


void do_send(int num_bytes, void* mess);

void do_recv();

bool get_start_game();

bool get_activate(int client_id);

int get_my_id();
int get_my_hp();

float get_MyPosition_x();
float get_MyPosition_y();

float get_Position_x(int id);
float get_Position_y(int id);

float get_Aim_x(int id);
float get_Aim_y(int id);

float get_bullet_x(int bullet_id);
float get_bullet_y(int bullet_id);
float get_bullet_active(int bullet_id);

// void return_otherPlayer(CPlayer** m_otherPlayer, ID3D12Device* m_pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);