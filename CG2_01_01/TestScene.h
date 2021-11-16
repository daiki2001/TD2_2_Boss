#pragma once
#include "AbstractScene.h"

#include "Player.h"
#include "Stage.h"
#include "Particle/Particle.h"

class TestScene
	:public AbstractScene
{
public:

	TestScene(IoChangedListener *impl);
	virtual ~TestScene() = default;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;

private:
	Player player;
	Stage stage;
	ParticleManager* particle1;
};

