#pragma once
#include "BaseEnemy.h"
class TestEnemy : public BaseEnemy
{
public:
	TestEnemy(Vector3 pos, float hp,float N , float e,float r) :
		BaseEnemy(pos, hp, N, e, r, ModelManager::ModelName::TestEnemy) {

	}

	void Initialize() override;
	void Update() override;

};

