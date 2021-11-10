#pragma once

#include "Object3d.h"
#include "Vector3.h"


class Player
{
public:
	Player();

	void Initialize();
	void Update();
	void Draw() const;


public:

	Vector3 pos;
private:
	void Attack();
	bool Move();
	void ChangeAngle();

private:
	Vector3 move;
	Vector3 rotate;			//�v���C���[�̌����x�N�g��
	Vector3 scale;			//�T�C�Y
	Object3d *object;

	const float radius = 5.0f;		//�v���C���[�̔��a
	Vector3 attackArea[8];	//�U������̂���ꏊ

	
	float moveSpeead;		//���݂̈ړ����x
	float atackSpeed;		//�U�����x

	//�X�e�[�^�X
	bool isAttack;	//�U��

	float energy;
};