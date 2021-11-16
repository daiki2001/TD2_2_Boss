#include "TestScene.h"
#include "Object3d.h"
#include "Collision.h"


TestScene::TestScene(IoChangedListener *impl)
	: AbstractScene(impl)
{
	stage.Initialize();
	player.Initialize();
	enemys.push_back(new TestEnemy({ 20,0,40 }, 7));
	enemys.push_back(new TestEnemy({ 100,0,100 }, 7));
	enemys.push_back(new TestEnemy({ 200 + 40,0,20 + 40 }, 7));
	
}

void TestScene::Initialize()
{
	stage.Initialize();
	player.Initialize();
	//“G‚ğ‚·‚×‚Ä‰Šú‰»
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

}
