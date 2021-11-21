#include "stdafx.h"
#include "Network.h"
//#include <WS2tcpip.h>

WSADATA wsa;
SOCKET sock;
SOCKADDR_IN serveraddr;
int my_id = 0;
int retval = 0;
char send_buf[BUFSIZE];
char recv_buf[BUFSIZE];
bool start_game = false;

struct NetPlayer {
	float x = 150;
	float y = 150;
	float aim_x = 150;
	float aim_y = 150;
};

array<NetPlayer, 4> mPlayer;

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	cout << "[" << msg << "] " << (char*)lpMsgBuf << endl;
	LocalFree(lpMsgBuf);
}

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	MessageBox(NULL, (LPTSTR)lpMsgBuf, (LPTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int netInit()
{
	const char* SERVERIP;
	char tempIP[16];
	SERVERIP = "127.0.0.1";

	// 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, SERVERIP, &server_addr.sin_addr);
	int ret = connect(sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	int err_num = WSAGetLastError();
	if (ret == SOCKET_ERROR) {
		int err_num = WSAGetLastError();
		if (WSA_IO_PENDING != err_num) {
			cout << " EROOR : Connect " << endl;
			err_quit("connect()");

		}
	}

	do_recv();
}

int netclose()
{
	// close socket()
	closesocket(sock);

	// 윈속종료
	WSACleanup();
	return 0;
}

void send_attack_packet()
{
	cs_packet_attack packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_ATTACK;
	do_send(sizeof(packet), &packet);
}

void send_move_packet(int direction)
{
	cout << "move" << endl;
	cs_packet_move packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MOVE;
	packet.direction = (char)direction;
	do_send(sizeof(packet), &packet);
}

void send_aim_packet(float x, float y)
{
	cs_packet_aim packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_AIM;
	packet.x = x;
	packet.y = y;
	do_send(sizeof(packet), &packet);
}

void do_send(int num_bytes, void* mess)
{
	ZeroMemory(&send_buf, sizeof(send_buf));
	memcpy(send_buf, mess, num_bytes);

	retval = send(sock, send_buf, num_bytes, 0);
	cout << "move" << sizeof(num_bytes) <<  endl;
	if (retval == SOCKET_ERROR) { err_display("SEND()"); }
}

void do_recv()
{
	//ZeroMemory(recv_buf, sizeof(recv_buf));
	retval = recv(sock, recv_buf, sizeof(recv_buf), 0);
	if (retval == SOCKET_ERROR) { err_display("RECV()");  return; }
	char* p = recv_buf;
	while (p < recv_buf + retval) {
		unsigned char packet_size = *p;
		char type = *(p + 1);
		if (packet_size <= 0) break;
		switch (type) {
		case SC_PACKET_LOGIN_OK: {
			sc_packet_login_ok* packet = reinterpret_cast<sc_packet_login_ok*>(p);
			my_id = packet->id;
			cout << my_id << endl;
			break;
		}
		case SC_PACKET_OTHER_INFO: {
			sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);
			// 다른 클라이언트를 그려주는 객체에 id부여
			// 다른 클라이언트 객체.id = packet->id;
			packet->id;
			cout << "other : "  << (int)packet->id << endl;
			break;
		}
		case SC_PACKET_START_GAME: {
			// 이제 렌더링을 시작
			start_game = true;
			break;
		}

		case SC_PACKET_MOVE: {
			sc_packet_move* packet = reinterpret_cast<sc_packet_move*> (p);
			int p_id = packet->id;
			
			// 각 알맞는 클라에 넣어주기
			
			int pos_x = packet->pos_x;
			int pos_y = packet->pos_y;
			int aim_x = packet->aim_x;
			int aim_y = packet->aim_y;

			mPlayer[p_id].x = pos_x;
			mPlayer[p_id].y = pos_y;
			mPlayer[p_id].aim_x = aim_x;
			mPlayer[p_id].aim_y = aim_x;

			cout << "[" << p_id << "] (pos_x, pos_y) :" << pos_x << "," << pos_y << "(aim_x, aim_y) : " << aim_x << "," << aim_y << endl;
			break;
		}
		case SC_PACKET_DEAD: {
			sc_packet_dead* packet = reinterpret_cast<sc_packet_dead*>(p);
			// 해당 id에 해당하는 id지워주기
			int p_id = packet->id;
			break;
		}
		case SC_PACKET_FIRE: {	// 이거 총알을 클라에서 계산하기로 했지만
			sc_packet_fire* packet = reinterpret_cast<sc_packet_fire*> (p);
			int p_id = packet->id;
			break;
		}
		case SC_PACKET_HIT: {
			sc_packet_hit* packet = reinterpret_cast<sc_packet_hit*> (p);
			int p_id = packet->id;
			// 나의 id면 hp유아이에서 체력 한칸을 없애주자
			break;
		}
		}
		p = p + packet_size;
	}
}

bool get_start_game()
{
	return start_game;
}

int get_my_id()
{
	return my_id;
}

float get_MyPosition_x()
{
	return mPlayer[my_id].x;
}

float get_MyPosition_y()
{
	return mPlayer[my_id].y;
}

float get_Aim_x(int id)
{
	return mPlayer[id].aim_x;
}

float get_Aim_y(int id)
{
	return mPlayer[id].aim_y;
}

float get_Position_x(int id)
{
	return mPlayer[id].x;
}

float get_Position_y(int id)
{
	return mPlayer[id].y;
}