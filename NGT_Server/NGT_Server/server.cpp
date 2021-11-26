#include"header.h"
#include"Player.h"
#include"Bullet.h"
#include"Wall.h"
// ��������

HANDLE			hEvent;				// �̺�Ʈ �ڵ�

int				thread_count = 0;	// ��� Ŭ���̾�Ʈ�� �����ߴ���(��� Ŭ���̾�Ʈ �����尡 �����������) �ľ�
bool			start_game = false;
const int		MAX_BULLET = 15;
unordered_map<int, Player>g_clients;
array<BulletObject, MAX_BULLET> bullets;
array<Wall, NOBJECTS> g_walls;

RECT obj_map[NOBJECTS] = {
		0,0,800,80,
		0,720,800,80,
		0,80,80,640,
		720,80,800,640,
		345,106,110,24,
		345,668,110,24,
		105,345,24,110,
		668,345,24,110,
		220,220,99,35,
		482,220,99,35,
		220,545,99,35,
		482,545,99,35,
		220,255,35,63,
		545,255,35,63,
		220,482,35,63,
		545,482,35,63
};
// �Լ�����
DWORD WINAPI	ProcessClient(LPVOID arg);			// Ŭ���̾�Ʈ ������
DWORD WINAPI	SendPacket(LPVOID arg);				// ���带 �ϴ� ������
void			gameStart();						// ���� ���� ó��
void			send_login_ok_packet(SOCKET* client_socket, int client_id);					// �α��� ������ �˷��ִ� ��Ŷ ����
void			send_other_info_packet(SOCKET* client_socket, int client_id, int other_id);	// �ٸ� Ŭ���̾�Ʈ�� ���� ��Ŷ ����
void			send_start_game_packet(SOCKET* client_socket, int client_id);				// ������ �����ϸ� ��� Ŭ���̾�Ʈ���� ��Ŷ ����	
void			send_move_packet(SOCKET* client_socket, int client_id);
void			send_dead_packet(SOCKET* client_socket, int client_id);
void			send_fire_packet(SOCKET* client_socket, int client_id, int bullet_id);
void			send_bullet_packet(SOCKET* client_socket, int bullet_id);
void			send_hit_packet(SOCKET* client_socket, int client_id, int bullet_id);
void			process_client(int client_id,char*p);
bool			collide(const RECT& rect1, const RECT& rect2);
bool			collide_bullet(const RECT& rect1, const POINT& bullet);

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
		if (client_sock== INVALID_SOCKET) { err_display("ACCEPT()"); break; }
		int id = thread_count + 1;
		Player* player = new Player(client_sock, id);
		g_clients.insert(make_pair(thread_count + 1, *player));
		// ���⼭ Thread������ ���� ����
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)player, 0, NULL);
		if (hThread == NULL) { closesocket(player->m_c_socket); }
		else { CloseHandle(hThread); }
		thread_count++;
	}

	// 3���� ���� �Ϸ�
	gameStart();
	HANDLE hSend;
	hThread = CreateThread(NULL, 0, SendPacket, NULL, 0, NULL);
	if (hThread == NULL) { cout << "������ ���� ����" << endl; }

	while (true) {
		//for (auto& cl:g_clients) {
		//	cl.second.update(0.0001f);
		//}
		for (int i = 0; i < MAX_BULLET; ++i) {
			BulletObject* bullet = &bullets[i];
			if (bullet->getActive()) {
				bullet->update(0.01f);
				POINT p;
				p.x = bullet->getPos_x();
				p.y = bullet->getPos_y();
				for (auto& cl : g_clients) {
					if (cl.second.active == false) continue;
					if (cl.second.m_id == bullet->getId()) continue;
					if (collide_bullet(cl.second.collision_rect, p)) {
						cl.second.m_hp--;
						for (int j = 1; j <= 3; ++j)
							send_hit_packet(&g_clients[j].m_c_socket, cl.second.m_id, i);
						if (cl.second.m_hp == 0) {
							cl.second.active = false;
							cl.second.m_pos_x = 4000;
							cl.second.m_pos_y = 4000;
							cl.second.update(0.001f);
							for (int j = 1; j <= 3; ++j)
								send_dead_packet(&g_clients[j].m_c_socket, cl.second.m_id);
						}
						bullet->setActive(false);
					}
				}
				for (auto& wall : g_walls) {
					if (collide_bullet(wall.collision_rect, p)){
						for (int j = 1; j <= 3; ++j)
							send_hit_packet(&g_clients[j].m_c_socket, 0, i);
						bullet->setActive(false);
					}
				}
			}
		}
		this_thread::sleep_for(10ms);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	cout << "����" << endl;
	int retval; // ���� ���� ����

	// ���޵� ���� ����
	Player* player =  (Player*)arg;
	SOCKET& client_sock = player->m_c_socket;
	SOCKADDR_IN client_addr;
	int addrlen;
	char* buf;
	int len;
	int id = player->m_id;

	buf=player->m_buf;

	len = BUFSIZE;

	cout << id << endl;
	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(client_addr);
	getpeername(client_sock, (SOCKADDR*)&client_addr, &addrlen);

	// ������ �ܼ�â�� ������ ����� �ּ� Ǯ��
	// printf("Ŭ���̾�Ʈ ���� %s : %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	send_login_ok_packet(&client_sock, id);
	for (auto& cl : g_clients) {
		if (cl.second.m_id == id) continue;
		send_other_info_packet(&cl.second.m_c_socket, cl.second.m_id, id);
		send_other_info_packet(&client_sock, id, cl.second.m_id);
	}

	// Ŭ���̾�Ʈ�� ������ ���
	while (1) {
		if (start_game == false) continue;
		retval = recv(client_sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			cout << id << "���� ���� ����" << endl;
			for (auto& cl : g_clients) {
				if (cl.second.m_id == id) continue;
				send_dead_packet(&cl.second.m_c_socket, id);
			}
		}
		process_client(id, buf);
		this_thread::sleep_for(10ms);
		if (g_clients[id].active == false) break;
	}

	while (1) {		// ������ �����ִ°��� �ޱ�� ������ ó���� ������ ����
		retval = recv(client_sock, buf, len, 0);
	}

	// ������ �ܼ�â�� ���� ���Ḧ ����� �ּ� Ǯ��
	// printf("Ŭ���̾�Ʈ ���� %s : %d \n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	closesocket(client_sock);
	return 0;
}

DWORD WINAPI SendPacket(LPVOID arg) {
	while (1) {
		for (auto& pl : g_clients) {
			for (int i = 1; i <= 3; ++i)
				send_move_packet(&pl.second.m_c_socket, i);
				
			// �Ѿ��� �ִٸ� �Ѿ˵� �����ش�
			for(int i=0; i< MAX_BULLET; ++i) {
				if (bullets[i].getActive()) {
					send_bullet_packet(&pl.second.m_c_socket, i);
				}
			}
		}
		this_thread::sleep_for(10ms);
	}
}

void gameStart()
{
	cout << "���ӽ���" << endl;
	// ���� �ʱ�ȭ
	g_clients[1].m_pos_x = 150;
	g_clients[1].m_pos_y = 150;
	g_clients[2].m_pos_x = 450;
	g_clients[2].m_pos_y = 150;
	g_clients[3].m_pos_x = 300;
	g_clients[3].m_pos_y = 400;
	for (auto& cl : g_clients) {
		send_start_game_packet(&cl.second.m_c_socket, cl.second.m_id);
		cl.second.update(0.001f);
	}
	for (int i = 0; i < NOBJECTS; ++i) {
		g_walls[i] =  Wall(obj_map[i].left, obj_map[i].top, obj_map[i].right, obj_map[i].bottom);
	}
	start_game = true;
}

void send_login_ok_packet(SOCKET* client_socket, int client_id)
{
	sc_packet_login_ok packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN_OK;
	packet.id = client_id;
	send(*client_socket, reinterpret_cast<char*>(&packet), sizeof(packet), 0);
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

void send_dead_packet(SOCKET* client_socket, int client_id)
{
	sc_packet_dead packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_DEAD;
	packet.id = client_id;
	send(*client_socket, reinterpret_cast<const char*>(&packet), packet.size, 0);
}

void send_fire_packet(SOCKET* client_socket, int client_id, int bullet_id)
{
	sc_packet_fire packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_FIRE;
	packet.id = client_id;
	packet.bullet_id = bullet_id;
	packet.x = bullets[bullet_id].getPos_x();
	packet.y = bullets[bullet_id].getPos_y();
	send(*client_socket, reinterpret_cast<const char*>(&packet), packet.size, 0);
}

void send_bullet_packet(SOCKET* client_socket, int bullet_id)
{
	sc_packet_bullet packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_BULLET;
	packet.id = bullet_id;
	packet.x = bullets[bullet_id].getPos_x();
	packet.y = bullets[bullet_id].getPos_y();
	send(*client_socket, reinterpret_cast<const char*>(&packet), packet.size, 0);
}

void send_hit_packet(SOCKET* client_socket, int client_id, int bullet_id)
{
	sc_packet_hit packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_HIT;
	packet.id = client_id;
	packet.bullet_id = bullet_id;
	send(*client_socket, reinterpret_cast<const char*>(&packet), packet.size, 0);
}

void process_client(int client_id, char* p)
{
	unsigned char packet_type = p[1];
	Player& cl = g_clients[client_id];
	switch (packet_type)
	{
	case CS_PACKET_MOVE:{
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(p);
		//cl.m_py = cl.m_pos_y;
		//cl.m_px = cl.m_pos_x;
		switch (packet->direction)//�̵�ó�� �浹üũ x
		{
		case 0:
			//cl.m_pos_y--;
			cl.m_dy-=cl.m_spd*0.001f;
			break;
		case 1:
			cl.m_dy += cl.m_spd * 0.001f;
			//cl.m_pos_y++; 
			break;
		case 2:
			cl.m_dx -= cl.m_spd * 0.001f;
			//cl.m_pos_x--; 
			break;
		case 3:
			cl.m_dx += cl.m_spd * 0.001f;
			//cl.m_pos_x++;
			break;
		default:
			cout << "�߸��Ȱ��� �Խ��ϴ� �����մϴ� " << client_id << endl;

			exit(-1);	

		}

		// cout <<  "[" << cl.m_id << "] x : " << cl.m_pos_x << "y :" << cl.m_pos_y << endl;
		cl.update(0.001f);
		for (auto& other : g_clients)
		{
			if (client_id == other.first)continue;
			if (collide(other.second.collision_rect,cl.collision_rect))
			{
				cout << "�浹" << endl;
				cl.setPrevPos();
			}
		}
		for (auto& wall : g_walls)
		{
			if (collide(wall.collision_rect, cl.collision_rect))
			{
				cout << "�� �浹" << endl;
				cl.setPrevPos();
			}
		}
		// �ӽ������� �� ������

		break;
	}
	case CS_PACKET_AIM: {
		cs_packet_aim* packet = reinterpret_cast<cs_packet_aim*>(p);
		cl.m_aim_x = packet->x;
		cl.m_aim_y = packet->y;
		break;
	}
	case CS_PACKET_ATTACK:
		for (int i = (cl.m_id - 1) * 5; i <= (cl.m_id * 5) - 1; ++i) {		//0~4 : 1���÷��̾� �Ѿ�, 5~9 : 1���÷��̾� �Ѿ�, 10~14 : 3���÷��̾� �Ѿ�,{
			BulletObject* bl = &bullets[i];
			if (bl->getActive() == false) {
				bl->setActive(true);
				bl->setId(cl.m_id);
				bl->setPos(cl.m_pos_x, cl.m_pos_y);
				
				POINT dir{ cl.m_aim_x - cl.m_pos_x, cl.m_aim_y - cl.m_pos_y };
				float temp = sqrt(dir.x * dir.x + dir.y * dir.y);
				float dx = dir.x / temp;
				float dy = dir.y / temp;
				bl->setDir(dx, dy);
				
				for (auto& pl : g_clients)
					send_fire_packet(&pl.second.m_c_socket, cl.m_id, i);
				break;
			}
		}
		break;
	}
}

bool collide(const RECT& rect1, const RECT& rect2)
{
	if (rect1.left > rect2.right) return false;
	if (rect1.top > rect2.bottom) return false;
	if (rect1.right < rect2.left) return false;
	if (rect1.bottom < rect2.top) return false;

	return true;
}

bool collide_bullet(const RECT& rect1, const POINT& bullet)
{
	if (bullet.y >= rect1.top && bullet.y <= rect1.bottom) {
		if (bullet.x <= rect1.right && bullet.x >= rect1.left) {
			return true;
		}
	}
	return false;
}
