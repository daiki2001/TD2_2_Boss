#pragma once
#include "BaseEnemy.h"
#include "Vector3.h"

class RushEnemy : public BaseEnemy
{
public:
	RushEnemy(Player *player, Vector3 pos, float hp, float N, float e, float r) :
		BaseEnemy(player, pos, hp, N, e, r, ModelManager::ModelName::TestEnemy) {
		flame = 0;
		playerPos = this->pos;
	}

	void Initialize() override;
	void Update() override;
	void RashStart(const Vector3& playerPos);

private:
	const size_t rushInterval = 60; //突進する間隔
	size_t flame; //フレーム
	Vector3 playerPos; //プレイヤーの座標

};
