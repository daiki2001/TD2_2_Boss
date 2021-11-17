#pragma once
#include "AbstractScene.h"

#include <vector>

#include "Player.h"
#include "BaseEnemy.h"
#include "TestEnemy.h"
#include "Stage.h"

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
	//Õ“ËŒãˆ—
	void Repulsion(float hitTime,Player &player,BaseEnemy &enemy);

	Player player;
	Stage stage;
	vector<BaseEnemy *>  enemys;
};

