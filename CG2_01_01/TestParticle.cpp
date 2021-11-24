#include "TestParticle.h"

TestParticle::TestParticle() :
	BaseParticle::BaseParticle()
{
}

TestParticle::~TestParticle()
{
	BaseParticle::~BaseParticle();
}

void TestParticle::Initialize()
{
	lifeTime = 60;
	startScale = 0.0f;
	endScale = 25.0f;
	startColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	endColor = { 0.0f, 1.0f, 1.0f, 1.0f };

	CreateManager("./Resources/effect1.png");
	UpdateCount.push_front(manager);
}

void TestParticle::Update(const bool& isCreate, const Vector3& offset)
{
	//manager->Update();

	if (isCreate)
	{
		static const float rnd_pos = 500.0f;
		static const float rnd_vel = 5.0f;
		static const float rnd_acc = 0.01f;

		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos += offset;

		speed.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		speed.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		speed.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		accel.z = -(float)rand() / RAND_MAX * rnd_acc;

		manager->Add(lifeTime, pos, speed, accel,
			startScale, endScale, startColor, endColor);
	}
}
