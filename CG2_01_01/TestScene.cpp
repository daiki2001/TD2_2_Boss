#include "TestScene.h"
#include "Object3d.h"
#include "Collision.h"
#include "../DirectX/Input/KeyboardInput.h"
#include "../DirectX/DirectXCommon/DirectXCommon.h"


TestScene::TestScene(IoChangedListener *impl)
	: AbstractScene(impl)
{
	stage.Initialize();
	player.Initialize();
	enemys.push_back(new TestEnemy({ 0,0,500 }, 7 ,				20.0f));
	enemys.push_back(new TestEnemy({ 100,0,100 }, 7,			20.0f));
	enemys.push_back(new TestEnemy({ 200 + 40,0,20 + 40 }, 7,	20.0f));
	particle1 = nullptr;
}

void TestScene::Initialize()
{
	stage.Initialize();
	player.Initialize();
	//敵をすべて初期化
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Initialize();
	}
	particle1 = ParticleManager::Create(L"Resources/effect1.png");
}

void TestScene::Finalize()
{
	delete particle1;
	particle1 = nullptr;
}

void TestScene::Update()
{
	stage.Update();
	player.Update();
	//敵をすべて更新
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
	}

	//当たり判定
	HitCollision();


	//移動を適用
	player.Reflection();
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Reflection();
	}
	
	Object3d::SetCamPos({ player.pos.x - 80.0f,200,player.pos.z-50.0f });
	Object3d::SetCamTarget({ player.pos.x - 80.0f,0.0f,player.pos.z});

	if (KeyboardInput::GetKeyPress(DIK_SPACE))
	{
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particle1->Add(60, pos, vel, acc, 1.0f, 0.0f, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	particle1->Update();

	Object3d::UpdateViewMatrix();
}

void TestScene::Draw() const
{
	stage.Draw();
	player.Draw();
	//敵をすべて描画
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw();
	}
	particle1->Draw(DirectXCommon::cmdList.Get());
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
	//衝突時点での位置
	Vector3 actPlayer = player.pos + player.move * hitTime;
	Vector3 actEnemy = enemy.pos + enemy.move * hitTime;
	//衝突地点
	Vector3 CollisionPos = actPlayer + (actEnemy - actPlayer) * player.r / (player.r + enemy.r);

	//位置決定
	//合計質量
	float TotalN = player.N + enemy.N;
	//反発率
	float RefRate = (1 + 0.5*0.5);
	//衝突軸
	Vector3 Direction = actEnemy - actPlayer;
	//ノーマライズ
	Direction.Normalize();
	//移動量の内積
	Vector3 moveVec = (player.move - enemy.move);
	float Dot = moveVec.VDot(Direction);
	//定数ベクトル
	Vector3 ConstVec = Direction * RefRate * Dot / TotalN;

	//衝突後の移動量
	player.move = ConstVec * -enemy.N + player.move;
	enemy.move = ConstVec * player.N + enemy.move;

	//衝突後位置
	player.pos = (player.move) * hitTime + actPlayer;
	enemy.pos = (enemy.move) * hitTime + actEnemy;

	if (player.move == Vector3{ 0,0,0 } &&
		enemy.move == Vector3{ 0,0,0 }) {
		
	}
}
