#include "GameObjCommon.h"


void GameObjCommon::Initialize()
{
}

void GameObjCommon::Update()
{
}

void GameObjCommon::Reflection()
{
}

void GameObjCommon::Draw() const
{
}

void GameObjCommon::Damage(float damage)
{
	hp -= damage;
	if (hp <= 5.0f) {
		hp = 5.0f;
		isAlive = false;
	}
}

void GameObjCommon::Dead()
{
}
