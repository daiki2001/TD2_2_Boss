#pragma once
#include "Vector3.h"
#include "Object3d.h"
#include "ModelManager.h"

class BaseEnemy
{
public:
	BaseEnemy(Vector3 startPos, float hp,float r, ModelManager::ModelName modelName) :
		pos(startPos),
		hp(hp),
		r(r),
		scale(Vector3{ hp,hp,hp }),
		move(Vector3{ 0,0,0 }),
		isAlive(true),
		N(8.0f)
	{
		object = nullptr;
		object = Object3d::Create();
		object->SetModel(ModelManager::GetIns()->GetModel(modelName));
		object->SetPos(pos);
		object->Update();
		Initialize();
	}

	virtual void Initialize();
	virtual void Update();
	virtual void Reflection();
	virtual void Draw() const;


public:
	enum State {
		STAY,
		ATTACK,
		DAMAGE,
	};
	State state;

public:
	Vector3 pos;	//座標
	Vector3 move;	//移動量
	Vector3 scale;	//サイズ
	float N;		//質量
	float r;		//半径
	bool isAlive;	//生存フラグ
	float hp;		//体力（サイズ）
protected:

	Vector3 rotate;			//エネミーの向きベクトル
	float angle = 0.0f;		//エネミーのy軸角度
	Object3d *object;

	//Vector3 attackArea[8];	//攻撃判定のある場所

	const float maxHp = 5.0f;	//体力最大
};

