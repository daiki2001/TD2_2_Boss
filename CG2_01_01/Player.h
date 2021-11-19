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
	//���b�N�I��
	void LockOn(const vector<GameObjCommon *> gameObj);
	//�ڐG��d������
	void Hit();
public:
	enum State {
		STAY,
		ATTACK,
		DAMAGE,
	};
	State state;

	bool isLockOn;	//���b�N�I���t���O
	bool isHit;
private:
	void Heal();
	void ChangeAngle(Vector3 targetPos,float ratio,Vector3 BaseAxis);
	bool VectorMove();
	bool XYMove();
	void Attack();

private:
	Object3d *frame;
	float moveSpeead;		//���݂̈ړ����x
	float atackSpeed;		//�U�����x
	int returnDamageCount;		//��_���[�W��Ԃ��畜�A����܂ł̃J�E���^�[
	int returnHitCount;		//��_���[�W��Ԃ��畜�A����܂ł̃J�E���^�[
	Vector3 stickRotate;	//���̓x�N�g��
};