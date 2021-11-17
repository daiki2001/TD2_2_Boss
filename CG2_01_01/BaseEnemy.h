#pragma once
#include "Vector3.h"
#include "Object3d.h"
#include "ModelManager.h"
#include "GameObjCommon.h"

class BaseEnemy : public GameObjCommon
{
public:
	BaseEnemy(Vector3 startPos, float hp, float N, float e, float r, ModelManager::ModelName modelName) :
		GameObjCommon(startPos,	hp,N,e,r,modelName)
	{
	}

	virtual void Initialize() override;
	virtual void Update()override;
	virtual void Reflection()override;
	virtual void Draw() const override;


public:
	enum State {
		STAY,
		ATTACK,
		DAMAGE,
	};
	State state;
  
};

