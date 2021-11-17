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
	enemys.push_back(new TestEnemy({ 20,0,40 }, 7));
	enemys.push_back(new TestEnemy({ 100,0,100 }, 7));
	enemys.push_back(new TestEnemy({ 200 + 40,0,20 + 40 }, 7));
	particle1 = nullptr;
}

void TestScene::Initialize()
{
	stage.Initialize();
	player.Initialize();
	//“G‚ğ‚·‚×‚Ä‰Šú‰»
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
	//“G‚ğ‚·‚×‚ÄXV
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
	}

	//“–‚½‚è”»’è
	HitCollision();

	player.Reflection();
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Reflection();
	}
	
	Object3d::SetCamPos({ player.pos.x - 80.0f,200,player.pos.z-50.0f });
	Object3d::SetCamTarget({ player.pos.x - 80.0f,0.0f,player.pos.z});

	if (KeyboardInput::GetKeyPress(DIK_SPACE))
	{
		// X,Y,Z‘S‚Ä[-5.0f,+5.0f]‚Åƒ‰ƒ“ƒ_ƒ€‚É•ª•z
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
	//“G‚ğ‚·‚×‚Ä•`‰æ
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw();
	}
}

void TestScene::HitCollision()
{
	for (int i = 0; i < enemys.size(); i++) {
		if (!Collision::IsPredictCollisionBall(player.pos, player.move, player.r*2, enemys[i]->pos, enemys[i]->move, enemys[i]->r*2)) {
			continue;
		}
		float hitTime = Collision::sphereSwept(player.pos, player.move, player.r, enemys[i]->pos, enemys[i]->move, enemys[i]->r);
		if (hitTime < 0) {

		}
	}
	particle1->Draw(DirectXCommon::cmdList.Get());
}
