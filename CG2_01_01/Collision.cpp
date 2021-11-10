#include "Collision.h"

bool Collision::IsBallToBallCollision(const Vector3& pos1, const float& r1, const Vector3& pos2, const float& r2)
{
	Vector3 distance = pos1 - pos2;

	return distance.Length() < (r1 + r2);
}
