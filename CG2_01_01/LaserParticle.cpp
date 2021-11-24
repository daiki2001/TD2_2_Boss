#include "LaserParticle.h"

LaserParticle::LaserParticle() :
	BaseParticle::BaseParticle(),
	normal(nullptr)
{
}

LaserParticle::~LaserParticle()
{
	BaseParticle::~BaseParticle();
}

void LaserParticle::Initialize(Vector3* normal)
{
	lifeTime = 120;
	accel = Vector3();
	startScale = 100.0f;
	endScale = startScale;

	this->normal = normal;

	CreateManager("./Resources/laser.png");
	UpdateCount.push_front(manager);
}

void LaserParticle::Update(const bool& isCreate, const Vector3& offset)
{
	if (isCreate)
	{
		static const float rnd_pos = 100.0f;
		static Vector3 angle = Vector3();

		angle = Vector3();
		angle.z = normal->VDot(Vector3(1.0f, 0.0f, 0.0f));
		if (normal->z < 0)
		{
			angle.z = +acos(angle.z) * 180.0f / XM_PI;
		}
		else
		{
			angle.z = -acos(angle.z) * 180.0f / XM_PI;
		}

		for (size_t i = 0; i < 3; i++)
		{
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos += offset;

			speed = *normal * 10.0f;

			startAngle = angle;
			endAngle = angle;

			startColor.x = (float)rand() / RAND_MAX * 0.75f + 1.0f - 0.75f;
			startColor.y = (float)rand() / RAND_MAX * 0.75f + 1.0f - 0.75f;
			startColor.z = (float)rand() / RAND_MAX * 0.75f + 1.0f - 0.75f;
			startColor.w = 1.0f;

			endColor.x = startColor.x;
			endColor.y = startColor.y;
			endColor.z = startColor.z;
			endColor.y = 0.5f;

			manager->Add(lifeTime, pos, speed, accel, startAngle, endAngle,
				startScale, endScale, startColor, endColor);
		}
	}
}
