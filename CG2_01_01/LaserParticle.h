#pragma once
#include "BaseParticle.h"
#include "Vector3.h"

class LaserParticle : public BaseParticle
{
public:
	LaserParticle();
	~LaserParticle();

public: // ƒƒ“ƒoŠÖ”
	void Initialize(Vector3* normal);
	void Update(const bool& isCreate, const Vector3& offset = Vector3()) override;

public: // ƒƒ“ƒo•Ï”
	Vector3* normal;

};
