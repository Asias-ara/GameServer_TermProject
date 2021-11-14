#include"header.h"
#include"Player.h"
// 전역변수
HANDLE			hEvent;				// 이벤트 핸들
int				thread_count = 0;	// 몇개의 클라이언트가 접속했는지(몇개의 클라이언트 쓰레드가 만들어졌는지) 파악
int				g_id = 0;			// 각 클라이언트에게 id부여
unordered_map<int, Player>g_clients;
// 함수선언
DWORD WINAPI	ProcessClient(LPVOID arg);			// 클라이언트 쓰레드
DWORD WINAPI	ControlClinet(LPVOID arg);			// 클라이언트를 제어하는 쓰레드
void			gameStart();						// 게임 시작 처리
void			send_login_ok_packet(SOCKET* client_socket, int client_id);					// 로그인 성공을 알려주는 패킷 전송
void			send_other_info_packet(SOCKET* client_socket, int client_id, int other_id);	// 다른 클라이언트의 정보 패킷 전송
void			send_start_game_packet(SOCKET* client_socket, int client_id);				// 게임이 시작하면 모든 클라이언트에게 패킷 전송	


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
			// 받을 데이터가 더 남아 있고 그때 해주어야 하는 것이 있을 때
		}
	}
	return (len - left);
}

int main(int argc, char* argv[])
{
	int retval;		// 오류 검출 변수

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
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

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;

	// 이벤트 사용 준비
	hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);		// 콘솔창에 쓰는것은 비신호로 시작
	if (hEvent == NULL) return 1;

	HANDLE hThread;
	while (thread_count < 3) {
		// accept()
		addrlen = sizeof(client_addr);
		client_sock = accept(sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET) { err_display("ACCEPT()"); break; }

		// 여기서 Thread생성후 소켓 전달
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
		thread_count++;
	}

	// 3명이 접속 완료
	gameStart();

	closesocket(sock);
	WSACleanup();
	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval; // 오류 검출 변수

	// 전달된 소켓 저장
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN client_addr;
	int addrlen;
	char* buf;
	int len;
	int id = thread_count;

	// 클라이언트 정보 얻기
	addrlen = sizeof(client_addr);
	getpeername(client_sock, (SOCKADDR*)&client_addr, &addrlen);

	// 서버의 콘솔창에 접속을 띄울경우 주석 풀기
	// printf("클라이언트 접속 %s : %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	// 클라이언트와 데이터 통신
	while (1) {
		WaitForSingleObject(hEvent, INFINITE);
		recv(client_sock, buf, len, 0);


		SetEvent(hEvent);
	}

	// 서버의 콘솔창에 접속 종료를 띄울경우 주석 풀기
	// printf("클라이언트 종료 %s : %d \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	closesocket(client_sock);
	return 0;
}

void gameStart()
{
	// 정보를 초기화 -> 처음 위치 어떻게 할 것인가?
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