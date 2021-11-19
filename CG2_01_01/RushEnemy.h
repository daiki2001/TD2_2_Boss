#pragma once
#include "BaseEnemy.h"
#include "Vector3.h"

class RushEnemy : public BaseEnemy
{
public:
	RushEnemy(Player *player, Vector3 pos, float hp, float N, float e) :
		BaseEnemy(player, pos, hp, N, e, ModelManager::ModelName::TestEnemy) {
		flame = 0;
	}

	void Initialize() override;
	void Update() override;
	void RushStart();

private:
	const size_t rushInterval = 120; //“Ëi‚·‚éŠÔŠu
	size_t flame; //ƒtƒŒ[ƒ€

};
