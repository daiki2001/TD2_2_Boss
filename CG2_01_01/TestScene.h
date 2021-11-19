#pragma once
#include "AbstractScene.h"

#include <vector>

#include "GameObjCommon.h"
#include "Player.h"
#include "BaseEnemy.h"
#include "TestEnemy.h"
#include "RushEnemy.h"
#include "Stage.h"
#include "BaseParticle.h"

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
	//���b�N�I��
	//�Փˏ���
	void HitCollision();
	//�Փˌ㏈��
	void Repulsion(float hitTime, GameObjCommon &a, GameObjCommon &b);


	Player player;
	Stage stage;
	vector<GameObjCommon *> enemys;
	BaseParticle particle;
};
