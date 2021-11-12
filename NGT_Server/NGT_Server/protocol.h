const char SERVERPORT = 49500;

// 프로토콜 설계
// 타입

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
const char SC_PACKET_HIT = 8;
// Packet(Application) 정의
#pragma pack(push, 1)


//클라에서 플레이어 이동 할때 보내는 패킷
struct cs_packet_move {
	char size;
	char	type;
	char	direction;			// 0 : up,  1: down, 2:left, 3:right
};

//클라에서 탱크가 바라보고 있는 방향을 보낼때 사용
struct cs_packet_aim
{
	char size;
	char type;
	float x;
	float y;
};


//플레이어가 공격
struct cs_packet_attack {
	char size;
	char type;
};



//--------------------------서버에서 보내는 것--------------------------


//처음 접속 했을때 접속한 클라이언트의 아이디를 보내준다.
struct sc_packet_login_ok {
	char size;
	char type;
	char id;
};

//접속한 다른 클라이언트의 정보를 보내준다
struct sc_packet_other_info {
	char size;
	char type;
	char id;
};

//전부 준비시 게임시작을 알림
struct sc_packet_start_game {
	char size;
	char type;
};


//오브젝트의 위치 수신
struct sc_packet_move {
	char size;
	char type;
	char id;
	float pos_x, pos_y;
	float aim_x, aim_y;
};

//플레이어의 사망시 화면에서 없애기위해 보내는 패킷
struct sc_packet_dead {
	char size;
	char type;
	char id;
};
//다른 플레이어가 공격했을 경우 누가 쐈는지 알려주는 패킷
struct sc_packet_fire {
	char size;
	char type;
	char id;
};

//서버에서 충돌처리를 통해 플레이어가 맞았을때 보내는 패킷
struct sc_packet_hit {
	char size;
	char type;
	char id;
};



//------------------------------------------------------




#pragma pack(pop)