#pragma once
#include "AbstractScene.h"

#include <vector>

#include "Player.h"
#include "BaseEnemy.h"
#include "TestEnemy.h"
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

	//Õ“Ëˆ—
	void HitCollision();

	Player player;
	Stage stage;
	vector<BaseEnemy *>  enemys;
	ParticleManager* particle1;
};

