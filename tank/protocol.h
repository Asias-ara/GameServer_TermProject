const short SERVERPORT = 9000;

// �������� ����
// Ÿ��


//
//const int CLIENT_WIDTH = 640;

//const int CLIENT_HEIGHT = 480;


//client to server
const char CS_PACKET_MOVE = 1;
const char CS_PACKET_AIM = 2;
const char CS_PACKET_ATTACK = 3;

//server to client
const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_OTHER_INFO = 2;
const char SC_PACKET_START_GAME = 3;
const char SC_PACKET_DEAD = 4;
const char SC_PACKET_MOVE = 5;
const char SC_PACKET_FIRE = 6;
const char SC_PACKET_AIM = 7;
const char SC_PACKET_BULLET = 8;
const char SC_PACKET_HIT = 9;
// Packet(Application) ����
#pragma pack(push, 1)


//Ŭ�󿡼� �÷��̾� �̵� �Ҷ� ������ ��Ŷ
struct cs_packet_move {
	char size;
	char	type;
	char	direction;			// 0 : up,  1: down, 2:left, 3:right
};

//Ŭ�󿡼� ��ũ�� �ٶ󺸰� �ִ� ������ ������ ���
struct cs_packet_aim
{
	char size;
	char type;
	float x;
	float y;
};


//�÷��̾ ����
struct cs_packet_attack {
	char size;
	char type;
};



//--------------------------�������� ������ ��--------------------------


//ó�� ���� ������ ������ Ŭ���̾�Ʈ�� ���̵� �����ش�.
struct sc_packet_login_ok {
	char size;
	char type;
	char id;
};

//������ �ٸ� Ŭ���̾�Ʈ�� ������ �����ش�
struct sc_packet_other_info {
	char size;
	char type;
	char id;
};

//���� �غ�� ���ӽ����� �˸�
struct sc_packet_start_game {
	char size;
	char type;
};


//������Ʈ�� ��ġ ����
struct sc_packet_move {
	char size;
	char type;
	char id;
	float pos_x, pos_y;
	float aim_x, aim_y;
};

//�÷��̾��� ����� ȭ�鿡�� ���ֱ����� ������ ��Ŷ
struct sc_packet_dead {
	char size;
	char type;
	char id;
};

//�ٸ� �÷��̾ �������� ��� ���� ������ �˷��ִ� ��Ŷ
struct sc_packet_fire {
	char size;
	char type;
	char id;
	char bullet_id;
	float x;
	float y;
};

struct sc_packet_bullet {
	char size;
	char type;
	char id;
	float x;
	float y;
};

//�������� �浹ó���� ���� �÷��̾ �¾����� ������ ��Ŷ
struct sc_packet_hit {
	char size;
	char type;
	char id;
	char bullet_id;
};
//------------------------------------------------------
#pragma pack(pop)