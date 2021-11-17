#include "TestScene.h"
#include "Object3d.h"
#include "Collision.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include <DirectXMath.h>


TestScene::TestScene(IoChangedListener *impl)
	: AbstractScene(impl)
{
	stage.Initialize();
	player.Initialize();
	//enemys.push_back(new TestEnemy({ 0,0,500 }, 7 ,				10.0f,0.5f,	20.0f));
	enemys.push_back(new TestEnemy({ -600,0,20 + 40 }, 20.0f, 100.0f, 0.5f, 60.0f));

}

void TestScene::Initialize()
{
	stage.Initialize();
	player.Initialize();
	//�G�����ׂď�����
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Initialize();
	}
}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	static int gameCounter = 0;	//�o�߃t���[���̃J�E���^�[
	gameCounter++;
	
	stage.Update();
	//�G�����ׂčX�V
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
	}
	//���b�N�I��
	if (ControllerInput::GetInstance()->GetPadButtonPress(XBOX_INPUT_RB)) {
		player.LockOn(enemys);
	}
	else {
		player.isLockOn = false;
	}
	player.Update();
	
	//�����蔻��
	HitCollision();


	//�ړ���K�p
	player.Reflection();
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Reflection();
	}
	
	Object3d::SetCamPos({ player.pos.x - 80.0f,400,player.pos.z-50.0f });
	Object3d::SetCamTarget({ player.pos.x - 80.0f,0.0f,player.pos.z});

	Object3d::UpdateViewMatrix();
}

void TestScene::Draw() const
{
	stage.Draw();
	player.Draw();
	//�G�����ׂĕ`��
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw();
	}
}

void TestScene::HitCollision()
{
	//�v���C���[�ƃG�l�~�[
	for (int i = 0; i < enemys.size(); i++) {
		if (!Collision::IsPredictCollisionBall(player.pos, player.move, player.r * 2, enemys[i]->pos, enemys[i]->move, enemys[i]->r * 2)) {
			continue;
		}
		float hitTime = Collision::sphereSwept(player.pos, player.move, player.r, enemys[i]->pos, enemys[i]->move, enemys[i]->r);
		if (hitTime < 0) {
			continue;
		}
		//�ǂ��炪�_���[�W�𕉂���
		if (player.move.Length() < enemys[i]->move.Length()) {
			player.Damage(enemys[i]->damage);
		}
		else {
		}
		Repulsion(hitTime, player, *enemys[i]);
		player.Hit();

	}

	//�G�l�~�[���m
	for (int l = 0; l < enemys.size(); l++) {
		for (int i = 0; i < enemys.size(); i++) {
			if (l >= i) {
				continue;
			}
			if (!Collision::IsPredictCollisionBall(enemys[l]->pos, enemys[l]->move, enemys[l]->r * 2, enemys[i]->pos, enemys[i]->move, enemys[i]->r * 2)) {
				//Vector3 check = enemys[i]->pos -player.pos;
				//float chackL = check.Length();
				continue;
			}
			float hitTime = Collision::sphereSwept(enemys[l]->pos, enemys[l]->move, enemys[l]->r, enemys[i]->pos, enemys[i]->move, enemys[i]->r);
			if (hitTime < 0) {
				continue;
			}
			//�Փˌ㏈��
			Repulsion(hitTime, *enemys[l], *enemys[i]);
		}
	}
}

void TestScene::Repulsion(float hitTime, GameObjCommon &a, GameObjCommon &b)
{
	//�Փˎ��_�ł̈ʒu
	Vector3 actPlayer = a.pos + a.move * (hitTime - 0.01f);
	Vector3 actEnemy = b.pos + b.move * (hitTime - 0.01f);
	//�Փ˒n�_
	Vector3 CollisionPos = actPlayer + (actEnemy - actPlayer) * a.r / (a.r + b.r);

	//�ʒu����
	//���v����
	float TotalN = a.N + b.N;
	//������
	float RefRate = (1 + b.e*0.5);
	//�Փˎ�
	Vector3 Direction = actEnemy - actPlayer;
	//�m�[�}���C�Y
	Direction.Normalize();
	//�ړ��ʂ̓���
	Vector3 moveVec = (a.move - b.move);
	float Dot = moveVec.VDot(Direction);
	//�萔�x�N�g��
	Vector3 ConstVec = Direction * RefRate * Dot / TotalN;

	//�Փˌ�̈ړ���
	a.move = ConstVec * -b.N + a.move;
	b.move = ConstVec * a.N + b.move;

	//�Փˌ�ʒu
	a.pos = (a.move) * hitTime + actPlayer;
	b.pos = (b.move) * hitTime + actEnemy;

	if (a.move == Vector3{ 0,0,0 } &&
		b.move == Vector3{ 0,0,0 }) {
		float Rand = rand() % 361 * XM_PI / 180.0f;
		b.pos += Vector3((a.r + b.r) * cosf(Rand), 0, (a.r + b.r) * sinf(Rand));
	}
}
