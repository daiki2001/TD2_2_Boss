#pragma once
#include "BaseParticle.h"
#include "Vector3.h"

class VacuumParticle : public BaseParticle
{
public:
	VacuumParticle();
	~VacuumParticle();

public: // �����o�֐�
	void Initialize(Vector3* normal);
	void Update(const bool& isCreate, const Vector3& offset, const float& range);

public: // �����o�ϐ�
	Vector3* normal;

};
