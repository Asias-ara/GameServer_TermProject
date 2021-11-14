#include"header.h"
#include"Player.h"
// ��������
HANDLE			hEvent;				// �̺�Ʈ �ڵ�
int				thread_count = 0;	// ��� Ŭ���̾�Ʈ�� �����ߴ���(��� Ŭ���̾�Ʈ �����尡 �����������) �ľ�
int				g_id = 0;			// �� Ŭ���̾�Ʈ���� id�ο�
unordered_map<int, Player>g_clients;
// �Լ�����
DWORD WINAPI	ProcessClient(LPVOID arg);			// Ŭ���̾�Ʈ ������
DWORD WINAPI	ControlClinet(LPVOID arg);			// Ŭ���̾�Ʈ�� �����ϴ� ������
void			gameStart();						// ���� ���� ó��
void			send_login_ok_packet(SOCKET* client_socket, int client_id);					// �α��� ������ �˷��ִ� ��Ŷ ����
void			send_other_info_packet(SOCKET* client_socket, int client_id, int other_id);	// �ٸ� Ŭ���̾�Ʈ�� ���� ��Ŷ ����
void			send_start_game_packet(SOCKET* client_socket, int client_id);				// ������ �����ϸ� ��� Ŭ���̾�Ʈ���� ��Ŷ ����	


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

int recvn(SOCKET s, char* buf, int len, int flags, int id)
{
	DWORD retval;
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR) return SOCKET_ERROR;
		else if (received == 0) break;

		left -= received;
		ptr += received;

		if (left > 0) {
			// ���� �����Ͱ� �� ���� �ְ� �׶� ���־�� �ϴ� ���� ���� ��
		}
	}
	return (len - left);
}

int main(int argc, char* argv[])
{
	int retval;		// ���� ���� ����

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("SOCKET()");

	// bind()
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(sock, (SOCKADDR*)&server_addr, sizeof(server_addr));
	if (retval == SOCKET_ERROR) err_quit("BIND()");

	// listen()
	retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("LISTEN()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;

	// �̺�Ʈ ��� �غ�
	hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);		// �ܼ�â�� ���°��� ���ȣ�� ����
	if (hEvent == NULL) return 1;

	HANDLE hThread;
	while (thread_count < 3) {
		// accept()
		addrlen = sizeof(client_addr);
		client_sock = accept(sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET) { err_display("ACCEPT()"); break; }

		// ���⼭ Thread������ ���� ����
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
		thread_count++;
	}

	// 3���� ���� �Ϸ�
	gameStart();

	closesocket(sock);
	WSACleanup();
	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval; // ���� ���� ����

	// ���޵� ���� ����
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN client_addr;
	int addrlen;
	char* buf;
	int len;
	int id = thread_count;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(client_addr);
	getpeername(client_sock, (SOCKADDR*)&client_addr, &addrlen);

	// ������ �ܼ�â�� ������ ����� �ּ� Ǯ��
	// printf("Ŭ���̾�Ʈ ���� %s : %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	// Ŭ���̾�Ʈ�� ������ ���
	while (1) {
		WaitForSingleObject(hEvent, INFINITE);
		recv(client_sock, buf, len, 0);


		SetEvent(hEvent);
	}

	// ������ �ܼ�â�� ���� ���Ḧ ����� �ּ� Ǯ��
	// printf("Ŭ���̾�Ʈ ���� %s : %d \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	closesocket(client_sock);
	return 0;
}

void gameStart()
{
	// ������ �ʱ�ȭ -> ó�� ��ġ ��� �� ���ΰ�?
}



void send_login_ok_packet(SOCKET* client_socket, int client_id)
{
	sc_packet_login_ok packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN_OK;
	packet.id = client_id;
	send(*client_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
}

void send_other_info_packet(SOCKET* client_socket, int client_id, int other_id)
{
	sc_packet_other_info packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_OTHER_INFO;
	packet.id = other_id;
	send(*client_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
}

void send_start_game_packet(SOCKET* client_socket, int client_id)
{
	sc_packet_start_game packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_START_GAME;
	send(*client_socket, reinterpret_cast<const char*>(&packet), sizeof(packet), 0);
}

void send_move_packet(SOCKET* client_socket, int client_id)
{
	sc_packet_move packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_MOVE;
	packet.pos_x = g_clients[client_id].m_pos_x;
	packet.pos_y = g_clients[client_id].m_pos_y;
	packet.aim_x = g_clients[client_id].m_aim_x;
	packet.aim_y = g_clients[client_id].m_aim_y;
	packet.id = client_id;
	send(*client_socket, reinterpret_cast<const char*>(&packet), packet.size, 0);
}