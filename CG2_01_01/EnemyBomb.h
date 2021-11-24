#pragma once
#include "GameObjCommon.h"
#include "Player.h"
#include "GameObjCommon.h"

class EnemyBomb :
	public GameObjCommon
{
public:
	EnemyBomb(Vector3 pos,Vector3 moveAngle, float hp, float N, float e) :
		GameObjCommon(pos, hp, N, e, hp * 1.5f, ModelManager::ModelName::BossBomb),
		shotAngle(moveAngle) {
		Initialize();
	}
	void static StaticInitialize(Player *player);
	void Initialize() override;
	void Update() override;
	void Damage(float damage) override;

private:
	void StandBomb();		//爆破前待機モーション
	void BombAttack();		//爆発
private:
	static Player *playerData;
	Vector3 shotAngle;
	bool BombStart;
	bool isBomb;
	float BombAnimationCounter;
	float BombAttackCounter;
	int counter;

};

