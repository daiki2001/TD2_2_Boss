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
	enemys.push_back(new TestEnemy	(&player, { -550,0,60 }, 10.0f, 10.0f, 2.0f));
	enemys.push_back(new TestEnemy	(&player, { -650,0,60 }, 10.0f, 10.0f, 2.0f));
	enemys.push_back(new TestEnemy	(&player, { -700,0,40 }, 10.0f, 10.0f, 2.0f));
	enemys.push_back(new TestEnemy	(&player, { -700,0,80 }, 10.0f, 10.0f, 2.0f));
	enemys.push_back(new TestEnemy	(&player, { -750,0,60 }, 10.0f, 10.0f, 2.0f));
	enemys.push_back(new TestEnemy	(&player, { -750,0,100 },10.0f, 10.0f, 2.0f));
	enemys.push_back(new TestEnemy	(&player, { -750,0,20 }, 10.0f, 10.0f, 2.0f));
	enemys.push_back(new RushEnemy	(&player, { 600,0,0 }, 10.0f, 100.0f, 0.5f));
	enemys.push_back(new Boss		(&player, { 1000,0,0 }, 10.0f, 100.0f, 0.5f));

}

void TestScene::Initialize()
{
	stage.Initialize();
	player.Initialize();
	//敵をすべて初期化
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Initialize();
	}
}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	static int gameCounter = 0;	//経過フレームのカウンター
	gameCounter++;
	
	stage.Update();
	//敵をすべて更新
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Update();
	}
	//for (int i = 0; i < rushEnemys.size(); i++) {
	//	rushEnemys[i]->Update();
	//	rushEnemys[i]->RashStart(player.pos);
	//}
	//ロックオン
	if (ControllerInput::GetInstance()->GetPadButtonPress(XBOX_INPUT_RB)) {
		player.LockOn(enemys);
	}
	else {
		player.isLockOn = false;
	}
	player.Update();
	
	//当たり判定
	HitCollision();


	//移動を適用
	player.Reflection();
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Reflection();
	}
	
	Object3d::SetCamPos({ player.pos.x - 80.0f,400,0.0f });
	Object3d::SetCamTarget({ player.pos.x - 80.0f,0.0f,50.0f });

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
}

void TestScene::HitCollision()
{
	//プレイヤーとエネミー
	for (int i = 0; i < enemys.size(); i++) {
		if (!Collision::IsPredictCollisionBall(player.pos, player.move, player.r * 2, enemys[i]->pos, enemys[i]->move, enemys[i]->r * 2)) {
			continue;
		}
		float hitTime = 0.0f;
		Vector3 collisionPos;
		Vector3 collisionPosA;
		Vector3 collisionPosB;
		if (Collision::sphereSwept(player.pos, player.move, player.r, enemys[i]->pos, enemys[i]->move, enemys[i]->r,
			hitTime,collisionPos,&collisionPosA,&collisionPosB)) {

			//どちらがダメージを負うか
			if (player.move.Length() < enemys[i]->move.Length()) {
				player.Damage(enemys[i]->damage);
			}
			else {
			}
			Bound(hitTime, player, *enemys[i],&collisionPosA,&collisionPosB);
			player.Hit();
		}

	}

	//エネミー同士
	for (int l = 0; l < enemys.size(); l++) {
		for (int i = 0; i < enemys.size(); i++) {
			if (l <= i) {
				continue;
			}
			if (!Collision::IsPredictCollisionBall(enemys[l]->pos, enemys[l]->move, enemys[l]->r * 2, enemys[i]->pos, enemys[i]->move, enemys[i]->r * 2)) {
				//Vector3 check = enemys[i]->pos -player.pos;
				//float chackL = check.Length();
				continue;
			}

			float hitTime = 0.0f;
			Vector3 collisionPos;
			Vector3 collisionPosA;
			Vector3 collisionPosB;
			if (Collision::sphereSwept(enemys[l]->pos, enemys[l]->move, enemys[l]->r, enemys[i]->pos, enemys[i]->move, enemys[i]->r,
				hitTime, collisionPos, &collisionPosA, &collisionPosB)) {
				//衝突後処理
				Bound(hitTime, *enemys[l], *enemys[i], &collisionPosA, &collisionPosB);
			}
		}
	}
}

void TestScene::Bound(float hitTime, GameObjCommon &a, GameObjCommon &b, Vector3 *collisionA, Vector3 *collisionB)
{
	
	//位置決定
	//合計質量
	float TotalN = a.N + b.N;
	//反発率
	float RefRate = (1 + b.e*player.e);
	//衝突軸
	Vector3 Direction = *collisionB - *collisionA;
	//ノーマライズ
	Direction.Normalize();
	//移動量の内積
	Vector3 moveVec = (a.move - b.move);
	float Dot = moveVec.VDot(Direction);
	//定数ベクトル
	Vector3 ConstVec = Direction * RefRate * Dot / TotalN;

	//衝突後の移動量
	a.move = ConstVec * -b.N + a.move;
	b.move = ConstVec * a.N + b.move;

	//衝突後位置
	a.pos = (a.move * hitTime) + *collisionA;
	b.pos = (b.move * hitTime) + *collisionB;

	if (a.move == Vector3{ 0,0,0 } &&
		b.move == Vector3{ 0,0,0 }) {
		float Rand = rand() % 361 * XM_PI / 180.0f;
		b.pos += Vector3((a.r + b.r) * cosf(Rand), 0, (a.r + b.r) * sinf(Rand));
	}
}
