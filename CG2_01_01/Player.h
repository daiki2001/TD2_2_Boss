#pragma once

#include "Object3d.h"
#include "Vector3.h"


class Player
{
public:
	Player();

	void Initialize();
	void Update();
	void Reflection();
	void Draw() const;

public:
	enum State {
		STAY,
		ATTACK,
		DAMAGE,
	};
	State state;

private:
	void Heal();
	void ChangeAngle();
	bool Move();
	void Attack();

public:
	Vector3 pos;
	Vector3 move;
	Vector3 scale;	//�T�C�Y
	float N;		//����
	float r;		//���a
private:

	Vector3 rotate;			//�v���C���[�̌����x�N�g��
	float angle = 0.0f;		//�v���C���[��y���p�x
	Object3d *object;

	Vector3 attackArea[8];	//�U������̂���ꏊ

	
	float moveSpeead;		//���݂̈ړ����x
	float atackSpeed;		//�U�����x

	const float maxHp = 5.0f;	//�̗͍ő�
	float hp;	//�̗́i�T�C�Y�j
};