#include "RushEnemy.h"

void RushEnemy::Initialize()
{
	flame = 0;
}

void RushEnemy::Update()
{
	switch (state)
	{
	case BaseEnemy::STAY:
		flame++;
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

	Reflection();
}

void RushEnemy::RashStart(const Vector3& playerPos)
{
	if (flame >= rushInterval)
	{
		flame = 0;
		state = ATTACK;
		this->playerPos = playerPos;
		rotate = this->playerPos - pos;
		rotate.Normalize();
		move = rotate * 30.0f;

		angle = rotate.VDot(Vector3(1.0f, 0.0f, 0.0f));
		if (rotate.z > 0)
		{
			angle = -acosf(angle) * 180 / XM_PI;
		}
		else
		{
			angle = +acosf(angle) * 180 / XM_PI;
		}
	}
}
