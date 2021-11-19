#pragma once
#include "AbstractScene.h"

#include <vector>

#include "GameObjCommon.h"
#include "Player.h"
#include "Boss.h"
#include "BaseEnemy.h"
#include "TestEnemy.h"
#include "RushEnemy.h"
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
	//ƒƒbƒNƒIƒ“
	//Õ“Ëˆ—
	void HitCollision();
	//Õ“ËŒãˆ—
	void Bound(float hitTime, GameObjCommon &a, GameObjCommon &b,Vector3 *collisionA, Vector3 *collisionB);


	Player player;
	Stage stage;
	vector<GameObjCommon *>  enemys;
};
