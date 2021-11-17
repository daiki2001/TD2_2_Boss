#include "TestScene.h"
#include "Object3d.h"
#include "Collision.h"


TestScene::TestScene(IoChangedListener *impl)
	: AbstractScene(impl)
{
	stage.Initialize();
	player.Initialize();
	enemys.push_back(new TestEnemy({ 0,0,500 }, 7 ,				20.0f));
	enemys.push_back(new TestEnemy({ 100,0,100 }, 7,			20.0f));
	enemys.push_back(new TestEnemy({ 200 + 40,0,20 + 40 }, 7,	20.0f));
	
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
	stage.Update();
	player.Update();
	//�G�����ׂčX�V
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
	}

	//�����蔻��
	HitCollision();


	//�ړ���K�p
	player.Reflection();
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Reflection();
	}
	
	Object3d::SetCamPos({ player.pos.x - 80.0f,200,player.pos.z-50.0f });
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
	for (int i = 0; i < enemys.size(); i++) {
		if (!Collision::IsPredictCollisionBall(player.pos, player.move, player.r*2, enemys[i]->pos, enemys[i]->move, enemys[i]->r*2)) {
			//Vector3 check = enemys[i]->pos -player.pos;
			//float chackL = check.Length();
			continue;
		}
		float hitTime = Collision::sphereSwept(player.pos, player.move, player.r, enemys[i]->pos, enemys[i]->move, enemys[i]->r);
		if (hitTime < 0) {
			return;
		}
		Repulsion(hitTime, player, *enemys[i]);
	}

}

void TestScene::Repulsion(float hitTime, Player &player, BaseEnemy &enemy)
{
	//�Փˎ��_�ł̈ʒu
	Vector3 actPlayer = player.pos + player.move * hitTime;
	Vector3 actEnemy = enemy.pos + enemy.move * hitTime;
	//�Փ˒n�_
	Vector3 CollisionPos = actPlayer + (actEnemy - actPlayer) * player.r / (player.r + enemy.r);

	//�ʒu����
	//���v����
	float TotalN = player.N + enemy.N;
	//������
	float RefRate = (1 + 0.5*0.5);
	//�Փˎ�
	Vector3 Direction = actEnemy - actPlayer;
	//�m�[�}���C�Y
	Direction.Normalize();
	//�ړ��ʂ̓���
	Vector3 moveVec = (player.move - enemy.move);
	float Dot = moveVec.VDot(Direction);
	//�萔�x�N�g��
	Vector3 ConstVec = Direction * RefRate * Dot / TotalN;

	//�Փˌ�̈ړ���
	player.move = ConstVec * -enemy.N + player.move;
	enemy.move = ConstVec * player.N + enemy.move;

	//�Փˌ�ʒu
	player.pos = (player.move) * hitTime + actPlayer;
	enemy.pos = (enemy.move) * hitTime + actEnemy;

	if (player.move == Vector3{ 0,0,0 } &&
		enemy.move == Vector3{ 0,0,0 }) {
		
	}
}
