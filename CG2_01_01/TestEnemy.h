#pragma once
#include "BaseEnemy.h"
class TestEnemy : public BaseEnemy
{
public:
	TestEnemy(Player *player,Vector3 pos, float hp,float N , float e) :
		BaseEnemy(player, pos, hp, N, e, ModelManager::ModelName::TestEnemy) {
		type = BaseEnemy::Test;
	}
	void Initialize() override;
	void Update() override;
	void Reflection()override;


};

