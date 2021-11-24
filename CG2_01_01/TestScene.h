#pragma once
#include "AbstractScene.h"

#include <vector>
#include "Sprite.h"

#include "GameObjCommon.h"
#include "Player.h"
#include "Boss.h"
#include "BaseEnemy.h"
#include "TestEnemy.h"
#include "RushEnemy.h"
#include "Stage.h"
#include "TestParticle.h"
#include "DeathParticle.h"

#include "Reticle.h"

class TestScene
	:public AbstractScene
{
public:

	TestScene(IoChangedListener *impl);
	virtual ~TestScene() = default;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;

private:
	//���b�N�I��
	//�Փˏ���
	void HitCollision();
	//�ǂƂ̏Փˏ���
	void HitWall();
	//�Փˌ㏈��
	void Bound(float hitTime, GameObjCommon &a, GameObjCommon &b,Vector3 *collisionA, Vector3 *collisionB);
	//�J�����A�b�v�f�[�g
	void UpdateCamera();
	//�J�����h��
	void Shake(float damage);

	Player player;
	Stage stage;
	Reticle reticle;

	//sprite
	Sprite *test;

	TestParticle testParticle;
	DeathParticle deathParticle;

	float shakeRange;		//�_���[�W�ɉ����ėh�炷���߂̕ϐ�
	bool isShake;			//�V�F�C�NFlag
	Vector3 shakePos;		//�h�ꕝ
};