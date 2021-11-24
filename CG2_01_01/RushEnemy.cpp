#include "RushEnemy.h"
#include "Collision.h"

void RushEnemy::Initialize()
{
	flame = 0;
}

void RushEnemy::Update()
{

	//Ž€–Sˆ—
	if (!isAlive) {
		Dead();
	}

	//ˆÚ“®—Ê‰Šú‰»
	if (move.Length() > 0.1f) {
		move = move * 0.95f;
	}
	else {
		move = { 0,0,0 };
	}


	switch (state)
	{
	case BaseEnemy::STAY:
		flame++;
		RushStart();
		break;
	case BaseEnemy::ATTACK:
		if (move.Length() < 1.0f)
		{
			state = STAY;
		}
		break;
	case BaseEnemy::DAMAGE:
		break;
	default:
		break;
	}


	if (isAlive) {
		scale = { hp,hp,hp };
	}
}

void RushEnemy::RushStart()
{
	Vector3 distance = playerData->pos - pos;

	if (distance.Length() > playerData->r + 1000.0f)
	{
		return;
	}

	if (flame >= rushInterval)
	{
		flame = 0;
		state = ATTACK;
		rotate = distance;
		rotate.Normalize();
		move = rotate * 20.0f;

		angle = rotate.VDot(Vector3(0.0f, 0.0f, 1.0f));
		if (rotate.x < 0)
		{
			angle = -acosf(angle) * 180 / XM_PI;
		}
		else
		{
			angle = +acosf(angle) * 180 / XM_PI;
		}
	}
}
