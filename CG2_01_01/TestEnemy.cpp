#include "TestEnemy.h"
#include "Easing.h"

void TestEnemy::Initialize()
{
	move = { 0,0,0 };
	particle.Initialize();
}


void TestEnemy::Update()
{
	//���S����
	if (!isAlive) {
		Dead();
	}

	//�ړ��ʏ�����
	if (move.Length() > 0.1f) {
		move = move * 0.95f;
	}
	else {
		move = { 0,0,0 };
	}


	//�ړ��K��
	pos += move;
	N = hp * 5;
	r = hp * 1.5f;
	//�_���[�W�����ʂƈړ����x����v�Z
	damage = N * move.Length() * 0.005f;

	if (isAlive) {
		scale = { hp,hp,hp };
	}

	particle.Update(isAlive == false, pos);
}

void TestEnemy::Reflection()
{
	object->SetScale(scale);
	object->SetPos(pos);
	object->Update();

}
