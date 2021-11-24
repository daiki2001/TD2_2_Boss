#pragma once
#include "BaseParticle.h"
#include "Vector3.h"

class VacuumParticle : public BaseParticle
{
public:
	VacuumParticle();
	~VacuumParticle();

public: // ƒƒ“ƒoŠÖ”
	void Initialize(Vector3* normal);
	void Update(const bool& isCreate, const Vector3& offset, const float& range);

public: // ƒƒ“ƒo•Ï”
	Vector3* normal;

};
