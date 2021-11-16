#pragma once
#include "BaseEnemy.h"
class TestEnemy : public BaseEnemy
{
public:
	TestEnemy(Vector3 pos, float hp) :
		BaseEnemy(pos, hp, ModelManager::ModelName::TestEnemy) {

	}

	void Initialize() override;
	void Update() override;

};

