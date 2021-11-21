#include "TestEnemy.h"
#include "Easing.h"

void TestEnemy::Initialize()
{
	move = { 0,0,0 };
}


void TestEnemy::Update()
{
	//死亡処理
	if (!isAlive) {
		Dead();
	}

	r = hp * 1.5f;
	//移動量初期化
	if (move.Length() > 0.1f) {
		move = move * 0.95f;
	}
	else {
		move = { 0,0,0 };
	}


	//移動適応
	pos += move;
	N = hp * 5;
	r = hp * 1.5f;
	//ダメージを質量と移動速度から計算
	damage = N * move.Length() * 0.005f;

	if (isAlive) {
		scale = { hp,hp,hp };
	}
}

void TestEnemy::Reflection()
{
	object->SetScale(scale);
	object->SetPos(pos);
	object->Update();

}
