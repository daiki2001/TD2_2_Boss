#include "RushEnemy.h"

void RushEnemy::Initialize()
{
	flame = 0;
}

void RushEnemy::Update()
{
	if (isAlive)
	{

	}

	switch (state)
	{
	case BaseEnemy::STAY:
		flame++;
		break;
	case BaseEnemy::ATTACK:
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
		rotate = pos - this->playerPos;
		rotate.Normalize();

		angle = rotate.VDot(Vector3(0.0f, 1.0f, 0.0f));
		if (rotate.x > 0)
		{
			angle = +acos(angle) * 180 / XM_PI;
		}
		else
		{
			angle = -acos(angle) * 180 / XM_PI;
		}
		int a = 0;
	}
}
