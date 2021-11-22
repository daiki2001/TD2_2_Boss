#pragma once
#include "BaseParticle.h"

class TestParticle : public BaseParticle
{
public:
	TestParticle();
	~TestParticle();

public:
	void Initialize() override;
	void Update(const bool& isCreate, const Vector3& offset = Vector3()) override;

};
