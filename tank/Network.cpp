#include "stdafx.h"
#include "Network.h"

WSADATA wsa;
SOCKET sock;
SOCKADDR_IN serveraddr;
int my_id = 0;
int retval = 0;
char send_buf[BUFSIZE];
char recv_buf[BUFSIZE];

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

	// ���� �ʱ�ȭ
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

	// ��������
	WSACleanup();
	return 0;
}

void do_send(int num_bytes, void* mess)
{
	ZeroMemory(&send_buf, sizeof(send_buf));
	memcpy(send_buf, mess, num_bytes);

	retval = send(sock, send_buf, num_bytes, 0);
	if (retval == SOCKET_ERROR) { err_display("SEND()"); }
}

void do_recv()
{
	retval = recv(sock, recv_buf, sizeof(recv_buf), 0);
	if (retval == SOCKET_ERROR) { err_display("RECV()");  return; }
	char* p = recv_buf;
	while (p < recv_buf + retval) {
		unsigned char packet_size = *p;
		int type = *(p + 1);
		if (packet_size <= 0) break;
		switch (type) {
		case SC_PACKET_LOGIN_OK: {
			sc_packet_login_ok* packet = reinterpret_cast<sc_packet_login_ok*>(p);
			my_id = packet->id;
			break;
		}
		case SC_PACKET_OTHER_INFO: {
			sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);
			// �ٸ� Ŭ���̾�Ʈ�� �׷��ִ� ��ü�� id�ο�
			// �ٸ� Ŭ���̾�Ʈ ��ü.id = packet->id;
			packet->id;
			break;
		}
		case SC_PACKET_START_GAME: {
			// ���� �������� ����
			break;
		}
		case SC_PACKET_MOVE: {
			sc_packet_move* packet = reinterpret_cast<sc_packet_move*> (p);
			int p_id = packet->id;

			// �� �˸´� Ŭ�� �־��ֱ�
			packet->pos_x;
			packet->pos_y;
			packet->aim_x;
			packet->aim_y;
			break;
		}
		case SC_PACKET_DEAD: {
			sc_packet_dead* packet = reinterpret_cast<sc_packet_dead*>(p);
			// �ش� id�� �ش��ϴ� id�����ֱ�
			int p_id = packet->id;
			break;
		}
		case SC_PACKET_FIRE: {	// �̰� �Ѿ��� Ŭ�󿡼� ����ϱ�� ������
			sc_packet_fire* packet = reinterpret_cast<sc_packet_fire*> (p);
			int p_id = packet->id;
			break;
		}
		case SC_PACKET_HIT: {
			sc_packet_hit* packet = reinterpret_cast<sc_packet_hit*> (p);
			int p_id = packet->id;
			// ���� id�� hp�����̿��� ü�� ��ĭ�� ��������
			break;
		}
		}
		p = p + packet_size;
	}
}
