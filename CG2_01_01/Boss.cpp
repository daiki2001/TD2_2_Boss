#include "Boss.h"
#include "DirectXCommon.h"
#include "Easing.h"


Boss::Boss(Player *player, Vector3 startPos, float hp, float N, float e) :
	BaseEnemy(player, startPos, hp, N, e, ModelManager::ModelName::BossCore) {

	frame = nullptr;
	frame = Object3d::Create();
	frame->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BossFrame));
	//frame->SetPos(object->GetPos());
	//frame->SetRotation(object->GetRotation());
	//frame->SetScale(object->GetScale());
	frame->SetParent(object);
	frame->Initialize();
	frame->Update();
	stayTimer = 120;
	standEaseTimer = 0;
	standMotionTimer = 0;
	Initialize();
}

void Boss::Initialize()
{
	move = { 0,0,0 };
	nextAttackType = Stay;
	stayTimer = 120;
	standEaseTimer = 0;
	standMotionTimer = 0;
	TackleCounter = 0;

}

void Boss::Update()
{
	//プレイヤーの向きベクトル
	playerVec = playerData->pos - pos;
	playerVec.Normalize();


	//死亡処理
	if (!isAlive) {
		Dead();
	}

	//移動量初期化
	if (move.Length() > 0.1f) {
		move = move * 0.95f;
	}
	else {
		move = { 0,0,0 };
	}

	//待機状態でタイマーが残っていれば減少させる
	if (state == Stay && stayTimer > 0) {
		stayTimer--;
	}
	//待機状態なら攻撃を決定する
	if (state == Stay && stayTimer == 0) {
		if (nextAttackType == Stay) {
			AttackSelect();
		}
		else {
			state = ATTACK;
		}
	}
	if (state == ATTACK) {

		//攻撃前モーションが終わったら攻撃開始
		if (StandbyMotion()) {
			switch (attackType)
			{
			case Boss::Stay:
				state = STAY;
				break;
				//通常のタックル
			case Boss::Tackle:
				stayTimer = 120;			//攻撃終了後のタイマーセット
				move += playerVec * 40;
				oldAtackType = Tackle;
				if(hp < 20){
					nextAttackType = Tackle3;
				}
				break;
			
				//三連タックル（HP20以下）
			case Boss::Tackle3:
				stayTimer = 30;			//攻撃終了後のタイマーセット
				TackleCounter++;
				move += playerVec * 10* (TackleCounter * 1.2f);
				oldAtackType = Tackle3;
				if (TackleCounter >= 3) {
					nextAttackType = Stay;
					TackleCounter = 0;
				}
				break;
			case Boss::Middle:
				state = STAY;

				break;
			case Boss::Long:
				state = STAY;
				break;
			default:
				break;
			}

			if (nextAttackType == Stay) {
				attackType = Stay;
			}
			else {
				attackType = nextAttackType;
			}
			state = STAY;
		}
	}

	//移動適応
	pos += move;
	N = hp * 3;
	r = hp * 1.5f;
	//ダメージを質量と移動速度から計算
	damage = N * move.Length() * 0.005f;

	//フレームの回転
	if (isAlive) {
		scale = { hp,hp,hp };
	}

}

void Boss::Reflection()
{
	object->SetScale(scale);
	object->SetPos(pos);
	object->Update();
	frame->SetRotation(frameRotate);
	frame->Update();
}

void Boss::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	frame->Draw();
	Object3d::PostDraw();
}

void Boss::AttackSelect()
{		//距離に応じて攻撃方法を選択
	state = ATTACK;
	if (ChackRange(100, 0)) {
		//attackType = Tackle;			//次の攻撃パターンを決定
	}
	else if (ChackRange(700, 450)) {
		attackType = Tackle;			//次の攻撃パターンを決定
	}
	else if (ChackRange(1000, 1000)) {
		//attackType = Long;			//次の攻撃パターンを決定
		//stayTimer = 240;			//攻撃終了後のタイマーセット
	}
	else {
		//該当距離の外側にいるときは行動しない
		attackType = Stay;

	}
}

bool Boss::ChackRange(float max, float min)
{
	Vector3 range = playerData->pos - pos;
	if (range.Length() <= max && range.Length() >= min) {
		return true;
	}
	return false;
}

Vector3 Boss::SpinFrame()
{
	if (frameRotate.x < 90) {
		frameRotate.x++;
		if (frameRotate.x >= 90) {
			frameRotate.x = 90;
		}
		return frameRotate;
	}
	if (frameRotate.y < 90) {
		frameRotate.y++;
		if (frameRotate.y >= 90) {
			frameRotate.y = 90;
		}
		return frameRotate;
	}
	frameRotate = { 0,0,0 };
	return frameRotate;
}

bool Boss::StandbyMotion()
{
	standEaseTimer++;
	standMotionTimer++;
	switch (attackType)
	{
	case Boss::Stay:
		standEaseTimer = 0;
		standMotionTimer = 0;
		return true;
		break;
	case Boss::Tackle3:
		return true;
		break;
	case Boss::Tackle:
		//回転量増加
		frameRotate.y += Ease(In, Back, (float)standEaseTimer / 30.0f, 0.0f, 15.0f);
		//後退
		if (standMotionTimer == 15) {
			move -= playerVec * 10.0f;
		}
		//回転
		if (standEaseTimer >= 30) {
			standEaseTimer = 30;
		}
		//攻撃開始
		if (standMotionTimer > 70) {
			frameRotate.y = 0.0f;
			standMotionTimer = 0;
			standEaseTimer = 0;
			return true;
		}
		break;
	case Boss::Middle:
		if (standMotionTimer > 120) {
			standMotionTimer = 0;
			standEaseTimer = 0;
			return true;
		}
		break;
	case Boss::Long:
		if (standMotionTimer > 120) {
			standMotionTimer = 0;
			standEaseTimer = 0;
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}