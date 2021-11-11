#pragma once
#include "Vector3.h"

namespace Collision
{
// ���̓��m�̓����蔻��
bool IsBallToBallCollision(const Vector3& pos1, const float& r1, const Vector3& pos2, const float& r2);

// �����̓��m�̓����蔻��
bool IsBoxToBoxCollision(const Vector3& pos1, const Vector3& rotation1, const Vector3& scale1, const Vector3& pos2, const Vector3& rotation2, const Vector3& scale2);
};
