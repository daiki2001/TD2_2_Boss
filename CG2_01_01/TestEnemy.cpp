#include "TestEnemy.h"

void TestEnemy::Initialize()
{
	move = { 0,0,0 };
}


void TestEnemy::Update()
{
	//�ړ��ʏ�����
	/*if (move.Length() > 0.1f) {
		move = move * 0.9f;
	}
	else {
		move = { 0,0,0 };
	}*/

	pos += move;

}

void TestEnemy::Reflection()
{
	object->SetScale(scale);
	object->SetPos(pos);
	object->Update();

}
