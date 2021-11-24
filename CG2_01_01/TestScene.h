#pragma once
#include "AbstractScene.h"

#include <vector>
#include "Sprite.h"

#include "GameObjCommon.h"
#include "Player.h"
#include "Boss.h"
#include "BaseEnemy.h"
#include "TestEnemy.h"
#include "RushEnemy.h"
#include "Stage.h"
#include "TestParticle.h"
#include "DeathParticle.h"

#include "Reticle.h"

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
	//ロックオン
	//衝突処理
	void HitCollision();
	//壁との衝突処理
	void HitWall();
	//衝突後処理
	void Bound(float hitTime, GameObjCommon &a, GameObjCommon &b,Vector3 *collisionA, Vector3 *collisionB);
	//カメラアップデート
	void UpdateCamera();
	//カメラ揺れ
	void Shake(float damage);

	Player player;
	Stage stage;
	Reticle reticle;

	//sprite
	Sprite *test;

	TestParticle testParticle;
	DeathParticle deathParticle;

	float shakeRange;		//ダメージに応じて揺らすための変数
	bool isShake;			//シェイクFlag
	Vector3 shakePos;		//揺れ幅
};