#pragma once

#include "Object3d.h"
#include "Vector3.h"


class Player
{
public:
	Player();

	void Initialize();
	void Update();
	void Draw() const;


public:

	Vector3 pos;
private:
	void Attack();
	bool Move();
	void ChangeAngle();

private:
	Vector3 move;
	Vector3 rotate;			//プレイヤーの向きベクトル
	Vector3 scale;			//サイズ
	Object3d *object;

	const float radius = 5.0f;		//プレイヤーの半径
	Vector3 attackArea[8];	//攻撃判定のある場所

	
	float moveSpeead;		//現在の移動速度
	float atackSpeed;		//攻撃速度

	//ステータス
	bool isAttack;	//攻撃

	float energy;
};