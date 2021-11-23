#pragma once

class BulletObject{
	int		id;		// ���� ���°�?		
	float	x, y;	// �Ѿ��� ��ġ
	float	spd;	// �Ѿ��� �ӵ�
	float	dx, dy;	// �Ѿ��� ����
	bool	actived;	// �����ϴ°� �ƴѰ�
public:
	BulletObject();

	void setDir(float _dx, float _dy);
	void setPos(float _x, float _y);
	void setActive();
	void setId(int client_id);

	float getPos_x();
	float getPos_y();
	bool getActive();
	int getId();

	 void update(float fTimeElapsed);
};
