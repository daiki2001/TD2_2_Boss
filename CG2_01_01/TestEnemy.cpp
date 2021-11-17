#include "TestEnemy.h"

void TestEnemy::Initialize()
{
	move = { 0,0,0 };
}

void TestEnemy::Update()
{
	//ˆÚ“®—Ê‰Šú‰»
	/*if (move.Length() > 0.1f) {
		move = move * 0.9f;
	}
	else {
		move = { 0,0,0 };
	}*/

	pos += move;

	object->SetScale(scale);
	object->Update();
}
