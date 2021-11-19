#pragma once

#include "Object3d.h"
#include "Vector3.h"

#include "GameObjCommon.h"

class Player : public GameObjCommon
{
public:
	Player();

	void Initialize()override;
	void Update()override;
	void Reflection()override;
	void Draw() const override;

	void Damage(float damage);
	//ロックオン
	void LockOn(const vector<GameObjCommon *> gameObj);
	//接触後硬直処理
	void Hit();
public:
	enum State {
		STAY,
		ATTACK,
		DAMAGE,
	};
	State state;

	bool isLockOn;	//ロックオンフラグ
	bool isHit;
private:
	void Heal();
	void ChangeAngle(Vector3 targetPos,float ratio,Vector3 BaseAxis);
	bool VectorMove();
	bool XYMove();
	void Attack();

private:
	Object3d *frame;
	float moveSpeead;		//現在の移動速度
	float atackSpeed;		//攻撃速度
	int returnDamageCount;		//被ダメージ状態から復帰するまでのカウンター
	int returnHitCount;		//被ダメージ状態から復帰するまでのカウンター
	Vector3 stickRotate;	//入力ベクトル
};