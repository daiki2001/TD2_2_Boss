#pragma once
#include "BaseEnemy.h"
class TestEnemy : public BaseEnemy
{
public:
	TestEnemy(Player *player,Vector3 pos, float hp,float N , float e,float r) :
		BaseEnemy(player, pos, hp, N, e, r, ModelManager::ModelName::TestEnemy) {

	}

	void Initialize() override;
	void Update() override;

};

