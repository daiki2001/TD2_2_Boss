#pragma once
#include "Vector3.h"
#include "Object3d.h"
#include "ModelManager.h"
#include "GameObjCommon.h"
#include "Player.h"
#include "BaseParticle.h"
#include "DeathParticle.h"

class BaseEnemy : public GameObjCommon
{
public:
	BaseEnemy(Player *player, Vector3 startPos, float hp, float N, float e,  ModelManager::ModelName modelName) :
		GameObjCommon(startPos, hp, N, e, hp * 1.5f, modelName)
	{
		playerData = player;
		deadCounter = 0.0f;
	}

	virtual void Initialize() override;
	virtual void Update()override;
	virtual void Reflection()override;
	virtual void Draw() const override;

	void Dead() override;


public:
	enum State {
		STAY,
		ATTACK,
		DAMAGE,
	};
	State state;

	enum TYPE {
		AttackArea,
		Test,
		Boss,
	};
	TYPE type;
	
	Player *playerData;

	//���S���̉��o�p�J�E���^�[
	float deadCounter;

	DeathParticle particle;

};

