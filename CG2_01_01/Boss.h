#pragma once
#include "BaseEnemy.h"
class Boss :
	public BaseEnemy
{
public:
	Boss(Player *player, Vector3 startPos, float hp, float N, float e);

	void Initialize()override;
	void Update()override;
	void Reflection()override;
	void Draw() const override;

private:
	Vector3 playerVec;	//�v���C���[�ւ̌����x�N�g��

	void Missile();
	void AttackSelect();				//�U�����@�I��
	bool ChackRange( float max, float min = 0.0f);	//�͈͓��`�F�b�N
	
	int stayTimer;
	Vector3 SpinFrame();				//�ҋ@���[�V����
	int standEaseTimer;		//�U���O���[�V�����C�[�W���O�p
	int standMotionTimer;	//�U���O���[�V�����^�C�}�[
	bool StandbyMotion();			//�U���O�ҋ@���[�V����
	void AttackMotion();			//�U�����ҋ@���[�V����

	//�U��������̑ҋ@���ԁA�U���I�����Ƃɐݒ肷��B

	Object3d *frame;
	Vector3 frameRotate;
	//�U�������ǂ���
	bool isAttack;
	//�e�X�g�p�A�^�b�N�X�e�[�g
	enum ATTACKTYPE {
		Stay,
		Tackle,
		Tackle3,
		Middle,
		Long,
	};
	ATTACKTYPE attackType;		//���̍U��
	ATTACKTYPE nextAttackType;	//���̍U��
	ATTACKTYPE oldAtackType;	//���O�̍U��

	int TackleCounter;			//�O�A�^�b�N���p�J�E���^

};

