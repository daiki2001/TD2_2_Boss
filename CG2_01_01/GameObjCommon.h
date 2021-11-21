#pragma once
#include "Vector3.h"
#include "ModelManager.h"
#include "Object3d.h"


class GameObjCommon
{
public:
	GameObjCommon(Vector3 startPos, float hp, float N, float e, float r, ModelManager::ModelName modelName) :
		pos(startPos),
		move( 0,0,0 ),
		rotate(0, 0, 0),
		hp(hp),
		r(r),
		scale(Vector3{ hp,hp,hp }),
		isAlive(true),
		isDelete(false),
		N(N),
		e(e),
		damage(1.0f) 
	{
		object = nullptr;
		object = Object3d::Create();
		object->SetModel(ModelManager::GetIns()->GetModel(modelName));
		object->SetPos(pos);
		object->SetRotation(rotate);
		object->SetScale(scale);
		object->Update();
		Initialize();
	}

	~GameObjCommon()
	{
		delete object;
		object = nullptr;
	}

	virtual void Initialize();
	virtual void Update();
	virtual void Reflection();
	virtual void Draw() const;

	void Damage(float damage);
	virtual void Dead();

public:
	Vector3 pos;	//���W
	Vector3 move;	//�ړ���
	Vector3 scale;	//�T�C�Y
	float N;		//����
	float e;		//�����W��
	float r;		//���a
	bool isAlive;	//�����t���O
	bool isDelete;	//�����t���O
	float hp;		//�̗́i�T�C�Y�j
	float damage;	//�ڐG���ɗ^����_���[�W
protected:

	Vector3 rotate;			//�G�l�~�[�̌����x�N�g��
	float angle = 0.0f;		//�G�l�~�[��y���p�x
	Object3d *object;
	const float maxHp = 10.0f;	//�̗͍ő�

};

