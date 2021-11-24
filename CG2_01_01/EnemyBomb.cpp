#include "EnemyBomb.h"
#include "Easing.h"
#include "Collision.h"
#include "AudioManager.h"

 Player *EnemyBomb::playerData;

void EnemyBomb::StaticInitialize(Player *player)
{
	EnemyBomb::playerData = player;
	
}

void EnemyBomb::Initialize()
{
	move = shotAngle * 30.0f;
	move.y = 3.0f;
	BombStart = false;
	isBomb = false;
	counter = 0;
	BombAttackCounter = 0.0f;
	damage = 0.0f;
}

void EnemyBomb::Update()
{
	if (!isAlive) {
		Dead();
	}
	Damage(0.05f);
	if (pos.y > 0) {
		move.y -= 0.1f;
	}
	if (pos.y < 0.0f) {
		pos.y = 0.0f;
	}

	//ˆÚ“®—Ê‰Šú‰»
	if (move.Length() > 0.1f) {
		move = move * 0.95f;
	}
	else {
		move = { 0,0,0 };
	}

	pos += move;
	r = hp * 1.5f;

	StandBomb();
	BombAttack();

	if (isAlive && !BombStart) {
		scale = { hp,hp,hp };
	}
}


void EnemyBomb::Damage(float damage)
{
	hp += damage;
	if (hp >= 20 && !BombStart) {
		hp = 20;
		BombStart = true;
		BombAnimationCounter = 0.0f;
	}
}

void EnemyBomb::StandBomb()
{
	if (!BombStart) return;

	BombAnimationCounter += 0.05f;
	if (BombAnimationCounter >= 1.0f) {
		BombAnimationCounter = 1.0f;
		isBomb = true;
	}
	scale = {
		(float)Ease(Out,Back,BombAnimationCounter,20.0f,40.0f),
		(float)Ease(Out,Back,BombAnimationCounter,20.0f,40.0f),
		(float)Ease(Out,Back,BombAnimationCounter,20.0f,40.0f),
	};
}

void EnemyBomb::BombAttack()
{
	if (!isBomb) return;
	if (BombAttackCounter <= 0.0f) {
		AudioManager::SoundPlayWave(AudioManager::Bomb, false);
	}
	BombAttackCounter += 0.2f;
	
	for (int i = 0; i < enemys.size(); i++) {
		if(Collision::IsBallToBallCollision(pos, 100, enemys[i]->pos, enemys[i]->r)) {
			Vector3 bombAngle = enemys[i]->pos - pos;
			bombAngle.Normalize();
			enemys[i]->move += bombAngle * 15;
			enemys[i]->Damage(10.0f);
		}
	}
	if (Collision::IsBallToBallCollision(pos, 100, playerData->pos, playerData->r)) {
		Vector3 bombAngle = playerData->pos - pos;
		bombAngle.Normalize();
		playerData->move += bombAngle * 20;
		playerData->Damage(10.0f);
	}
	if (BombAttackCounter >= 1.0f) {
		isBomb = false;
		isAlive = false;
		isDelete = true;
	}

}
