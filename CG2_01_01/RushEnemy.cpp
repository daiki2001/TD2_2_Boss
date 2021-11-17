#include "RushEnemy.h"

void RushEnemy::Initialize()
{
}

void RushEnemy::Update()
{
	pos += move;

	if (isAlive)
	{

	}

	//rotate.Normalize();

	Reflection();
}
