#pragma once
#include "BaseEnemy.h"
class TestEnemy : public BaseEnemy
{
public:
	TestEnemy(Vector3 pos, float hp,float r) :
		BaseEnemy(pos, hp, r, ModelManager::ModelName::TestEnemy) {

	}

	void Initialize() override;
	void Update() override;

};

