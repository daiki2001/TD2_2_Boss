#pragma once
#include "Vector3.h"

class Collision
{
public:
	// ���̓��m�̓����蔻��
	static bool IsBallToBallCollision(const Vector3& pos1, const float& r1, const Vector3& pos2, const float& r2);
};
