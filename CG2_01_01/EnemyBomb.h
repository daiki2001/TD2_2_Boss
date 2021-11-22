#pragma once
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
	void Initialize() override;
	void Update() override;
	void Damage(float damage) override;

private:
	Vector3 shotAngle;
	bool isBomb;
	int counter;

};

