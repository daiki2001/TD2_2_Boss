#pragma once
#include "Vector3.h"

class Collision
{
public:
	// ‹…‘Ì“¯Žm‚Ì“–‚½‚è”»’è
	static bool IsBallToBallCollision(const Vector3& pos1, const float& r1, const Vector3& pos2, const float& r2);
};
