#include "RushEnemy.h"
#include "Collision.h"

void RushEnemy::Initialize()
{
	flame = 0;
	particle.Initialize();
}

void RushEnemy::Update()
{
	//éÄñSèàóù
	if (!isAlive) {
		Dead();
	}

	switch (state)
	{
	case BaseEnemy::STAY:
		flame++;
		RushStart();
		break;
	case BaseEnemy::ATTACK:
		move = move * (9.0f / 10.0f);
		if (move.Length() < 1.0f)
		{
			move = { 0.0f, 0.0f, 0.0f };
			state = STAY;
		}
		break;
	case BaseEnemy::DAMAGE:
		break;
	default:
		break;
	}

	pos += move;

	if (isAlive) {
		scale = { hp,hp,hp };
	}

	particle.Update(isAlive == false, pos);
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
		move = rotate * 10.0f;

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
