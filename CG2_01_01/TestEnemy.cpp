#include "TestEnemy.h"

void TestEnemy::Initialize()
{
}

void TestEnemy::Update()
{
	object->SetScale(scale);
	object->Update();
}
