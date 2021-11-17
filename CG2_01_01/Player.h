#pragma once

#include "Object3d.h"
#include "Vector3.h"


class Player
{
public:
	Player();

	void Initialize();
	void Update();
	void Reflection();
	void Draw() const;

public:
	enum State {
		STAY,
		ATTACK,
		DAMAGE,
	};
	State state;

private:
	void Heal();
	void ChangeAngle();
	bool Move();
	void Attack();

public:
	Vector3 pos;
	Vector3 move;
	Vector3 scale;	//サイズ
	float N;		//質量
	float r;		//半径
private:

	Vector3 rotate;			//プレイヤーの向きベクトル
	float angle = 0.0f;		//プレイヤーのy軸角度
	Object3d *object;

	Vector3 attackArea[8];	//攻撃判定のある場所

	
	float moveSpeead;		//現在の移動速度
	float atackSpeed;		//攻撃速度

	const float maxHp = 5.0f;	//体力最大
	float hp;	//体力（サイズ）
};