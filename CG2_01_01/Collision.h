#pragma once
#include "Vector3.h"

namespace Collision
{
// ‹…‘Ì“¯m‚Ì“–‚½‚è”»’è
bool IsBallToBallCollision(const Vector3& pos1, const float& r1, const Vector3& pos2, const float& r2);

// ’¼•û‘Ì“¯m‚Ì“–‚½‚è”»’è
bool IsBoxToBoxCollision(const Vector3& pos1, const Vector3& rotation1, const Vector3& scale1, const Vector3& pos2, const Vector3& rotation2, const Vector3& scale2);

// Õ“Ë—\‘ª
// ” ‚Ìê‡‚ÍAVector3(‰¡•, ‚‚³, ‰œs).Length() / 2‚ğ“ü‚ê‚ê‚Îg‚¦‚Ü‚·
bool IsPredictCollisionBall(const Vector3& pos1, const Vector3& speed1, const float& r1, const Vector3& pos2, const Vector3& speed2, const float& r2);
};
