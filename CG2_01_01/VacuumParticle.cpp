#include "VacuumParticle.h"

VacuumParticle::VacuumParticle() :
	BaseParticle::BaseParticle(),
	normal(nullptr)
{
}

VacuumParticle::~VacuumParticle()
{
	BaseParticle::~BaseParticle();
}

void VacuumParticle::Initialize(Vector3* normal)
{
	lifeTime = 30;
	startAngle = Vector3();
	endAngle = Vector3();
	startScale = 100.0f;
	endScale = 0.0f;
	startColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	endColor = startColor;

	this->normal = normal;

	CreateManager("./Resources/effect1.png");
	UpdateCount.push_front(manager);
}

void VacuumParticle::Update(const bool& isCreate, const Vector3& offset, const float& range)
{
	if (isCreate)
	{
		static const float rnd_pos = 400.0f;
		static const float distanse = 400.0f;
		Vector3 v = (*normal) * distanse;
		Vector3 vN = v;
		vN.Normalize();
		Vector3 vp = Vector3();

		for (size_t i = 0; i < 3; i++)
		{
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos += offset + (*normal) * range;

			vp = pos - v;
			speed = (v * vN.VDot(vp) / v.Length() - vp) / lifeTime;
			accel = -v / distanse;

			manager->Add(lifeTime, pos, speed, accel, startAngle, endAngle,
				startScale, endScale, startColor, endColor);
		}
	}
}
