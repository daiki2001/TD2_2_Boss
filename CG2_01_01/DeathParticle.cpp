#include "DeathParticle.h"

DeathParticle::DeathParticle() :
	BaseParticle::BaseParticle()
{
}

DeathParticle::~DeathParticle()
{
	BaseParticle::~BaseParticle();
}

void DeathParticle::Initialize()
{
	lifeTime = 30;
	pos = Vector3();
	accel = Vector3();
	startScale = 25.0f;
	endScale = 0.0f;
	startColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	endColor = { 1.0f, 0.75f, 0.0f, 1.0f };

	CreateManager("./Resources/effect1.png");
}

void DeathParticle::Update(const bool& isCreate, const Vector3& offset)
{
	manager->Update();

	if (isCreate)
	{
		static const float rnd_pos = 100.0f;
		static const float pos_range = 50.0f;
		static const float vel = 5.0f;

		for (size_t i = 0; i < 10; i++)
		{
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.Normalize();
			pos = pos * pos_range;

			speed = pos;
			speed.Normalize();
			speed = speed * vel;

			pos += offset;

			manager->Add(lifeTime, pos, speed, accel,
				startScale, endScale, startColor, endColor);
		}
	}
}
