#include "EnemyBomb.h"

void EnemyBomb::Initialize()
{
	move = shotAngle * 40.0f;
	move.y = 3.0f;
	isBomb = false;
	counter = 0;
}

void EnemyBomb::Update()
{
	if (!isAlive) {
		Dead();
	}

	if (pos.y > 0) {
		move.y -= 0.1f;
	}
	if (pos.y < 0.0f) {
		pos.y = 0.0f;
	}

	//ˆÚ“®—Ê‰Šú‰»
	if (move.Length() > 0.1f) {
		move = move * 0.95f;
	}
	else {
		move = { 0,0,0 };
	}

	pos += move;
	r = hp * 1.5f;

	if (isAlive) {
		scale = { hp,hp,hp };
	}
}


void EnemyBomb::Damage(float damage)
{
	hp += damage;
	if (hp >= 20) {
		hp = 20;
	}
}
