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
	int counter;
	void AttackSelect();
	bool ChackRange( float max, float min = 0.0f);
	
	Object3d *frame;
	Vector3 frameRotate;
	Vector3 spinFrame();
	//�U�������ǂ���
	bool isAttack;
	//�e�X�g�p�A�^�b�N�X�e�[�g
	enum ATTACKSTATE {
		Short,
		Middle,
		Long,
	};
	ATTACKSTATE atackState;
};

