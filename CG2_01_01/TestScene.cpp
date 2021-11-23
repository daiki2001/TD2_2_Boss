#include "TestScene.h"
#include "Object3d.h"
#include "Collision.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include <DirectXMath.h>
#include "LoadStage.h"
#include "Easing.h"

TestScene::TestScene(IoChangedListener *impl)
	: AbstractScene(impl)
{
	stage.Initialize();
	player.Initialize();
	//enemys.push_back(new TestEnemy({ 0,0,500 }, 7 ,				10.0f,0.5f,	20.0f));
	LoadStage::LoadStageEnemy("./Resources/testStageEnemy.csv", enemys, &player);
	testParticle.Initialize();
}

void TestScene::Initialize()
{
	stage.Initialize();
	player.Initialize();
	//敵をすべて初期化
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Initialize();
	}
	testParticle.Initialize();
	shakePos = { 0,0,0 };
	isShake = false;
}

void TestScene::Finalize()
{
	testParticle.Finalize();
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
	//敵の削除
	for (int i = enemys.size() - 1; i >= 0; i--) {
		if (enemys[i]->isDelete) {
			delete enemys[i];//newはdeleteしてから消す
			enemys.erase(enemys.begin() + i);
		}
	}

	//ロックオン
	if (ControllerInput::GetInstance()->GetPadButtonPress(XBOX_INPUT_RB)) {
		player.LockOn(enemys);
	}
	else {
		player.isLockOn = false;
	}
	player.Update();
	testParticle.Update(KeyboardInput::GetKeyPress(DIK_SPACE));
	
	//当たり判定
	HitCollision();


	//移動を適用
	player.Reflection();
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Reflection();
	}
	
	UpdateCamera();
}

void TestScene::Draw() const
{
	stage.Draw();
	player.Draw();
	//敵をすべて描画
	for (int i = 0; i < enemys.size(); i++) {
		enemys[i]->Draw();
	}
	testParticle.Draw();
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
			isShake = true;
			//どちらがダメージを負うか
			if (player.move.Length() < enemys[i]->move.Length()) {
				player.Damage(enemys[i]->damage);
				shakeRange = enemys[i]->damage;
			}
			else {
				enemys[i]->Damage(player.damage);
				shakeRange = player.damage;
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
				isShake = true;
				shakeRange = (enemys[l]->damage + enemys[i]->damage) / 2;
				enemys[i]->Damage(enemys[l]->damage);
				enemys[l]->Damage(enemys[i]->damage);
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

void TestScene::UpdateCamera()
{
	Vector3 CamPos = {
		(float)Ease(In,Linear,0.2f,Object3d::GetCamPos().x,player.pos.x),
		800,
		(float)Ease(In,Linear,0.2f,Object3d::GetCamPos().z,player.pos.z),
	};
	Vector3 CamTarget = CamPos;
	CamTarget.y = 0.0f;
	CamTarget.z +=50;
	//カメラの揺れ
	Shake(shakeRange);

	Object3d::SetCamPos(CamPos+shakePos);
	Object3d::SetCamTarget(CamTarget +shakePos);
	ParticleManager::SetEye(CamPos +shakePos);
	ParticleManager::SetTarget(CamTarget +shakePos);

	Object3d::UpdateViewMatrix();
}

void TestScene::Shake(float damage)
{
	if (!isShake)	return;

	static float shakeCounter = 1.0f;
	shakeCounter-= 0.05f;
	float shakeRange = damage * 10;
	shakePos = {
		(rand() % (int)(shakeRange + 1) - (shakeRange + 1)/2) * shakeCounter,
		(rand() % (int)(shakeRange + 1) - (shakeRange + 1)/2) * shakeCounter,
		(rand() % (int)(shakeRange + 1) - (shakeRange + 1)/2) * shakeCounter,
	};
	if (shakeCounter <= 0.0f) {
		shakeCounter = 1.0f;
		isShake = false;
	}
}
