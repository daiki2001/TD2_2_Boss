#pragma once
#include "Vector3.h"

namespace Collision
{
// ‹…‘Ì“¯Žm‚Ì“–‚½‚è”»’è
bool IsBallToBallCollision(const Vector3& pos1, const float& r1, const Vector3& pos2, const float& r2);

// ’¼•û‘Ì“¯Žm‚Ì“–‚½‚è”»’è
bool IsBoxToBoxCollision(const Vector3& pos1, const Vector3& rotation1, const Vector3& scale1, const Vector3& pos2, const Vector3& rotation2, const Vector3& scale2);
};
