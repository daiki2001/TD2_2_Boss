#pragma once
#include "Vector3.h"
#include "Object3d.h"
#include "ModelManager.h"

class BaseEnemy
{
public:
	BaseEnemy(Vector3 startPos, float hp, float r, ModelManager::ModelName modelName) :
		pos(startPos),
		hp(hp),
		scale(Vector3{ hp,hp,hp }),
		isAlive(true)
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
	Vector3 pos;	//���W
	Vector3 move;	//�ړ���
	float r;		//���a
	bool isAlive;	//�����t���O
	float hp;		//�̗́i�T�C�Y�j
protected:

	Vector3 rotate;			//�G�l�~�[�̌����x�N�g��
	float angle = 0.0f;		//�G�l�~�[��y���p�x
	Vector3 scale;			//�T�C�Y
	Object3d *object;

	//Vector3 attackArea[8];	//�U������̂���ꏊ

	const float maxHp = 5.0f;	//�̗͍ő�
};

