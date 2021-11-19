#pragma once
#include "Vector3.h"
#include "Object3d.h"
#include "ModelManager.h"
#include "GameObjCommon.h"
#include "Player.h"

class BaseEnemy : public GameObjCommon
{
public:
	BaseEnemy(Player *player, Vector3 startPos, float hp, float N, float e,  ModelManager::ModelName modelName) :
		GameObjCommon(startPos, hp, N, e, hp * 1.5f, modelName)
	{
		playerData = player;
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
	Player *playerData;
};

