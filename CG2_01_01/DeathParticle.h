#pragma once
#include "BaseParticle.h"

class DeathParticle : public BaseParticle
{
public:
	DeathParticle();
	~DeathParticle();

public:
	void Initialize() override;
	void Update(const bool& isCreate, const Vector3& offset = Vector3()) override;

};
