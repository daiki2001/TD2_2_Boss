#include "Player.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Easing.h"
#include "Collision.h"
#include "yMath.h"


Player::Player():
	GameObjCommon(Vector3{0,0,0},20.0f,100.0f,0.1f,25, ModelManager::PlayerCore)
{
	//フレーム
	frame = nullptr;
	frame = Object3d::Create();
	frame->SetModel(ModelManager::GetIns()->GetModel(ModelManager::PlayerFrame));
	frame->SetPos(object->GetPos());
	frame->SetRotation(object->GetRotation());
	frame->SetScale(object->GetScale());
	frame->Initialize();
	frame->Update();

	Initialize();
}

void Player::Initialize()
{
	hp = maxHp;
	r = hp;
	N = 10.0f;
	pos = { 0.0f, 0.0f, 0.0f };
	scale = { hp,hp,hp };
	move = { 0.0f, 0.0f, 0.0f };
	rotate = { 0.0f,0.0f,1.0f };
	moveSpeead = 0.0f;
	atackSpeed = 0.0f;
	isLockOn = false;
	isHit = false;
	state = STAY;
}

void Player::Update()
{
	//入力を最新に
	stickRotate = {						//現在の入力向きベクトル
		(float)ControllerInput::IsPadStick(INPUT_AXIS_X,0.2f),
		-(float)ControllerInput::IsPadStick(INPUT_AXIS_Y,0.2f),
		0.0f
	};
	//移動量初期化
	if(move.Length() > 0.1f){
		move = move * 0.95f;
	}
	else {
		move = { 0,0,0 };
	}
	
	Heal();			//自動回復
	//ロックオンせずに移動している場合更新
	if (stickRotate != Vector3{ 0.0f, 0.0f, 0.0f } && !isLockOn){
		ChangeAngle(stickRotate, 0.1f, Vector3(0, 1, 0));	//角度変更
	}
	//接触後硬直がなければ移動と攻撃可能
	if(!isHit){
		if(isLockOn){
			XYMove();
		}
		else {
			VectorMove();			//移動
		}
	}
	Attack();



	//移動適応
	move.y = 0.0f;	//yを無効化
	pos += move;
	//HPを質量に適応
	N = hp * 5;
	r = hp * 1.5f;

	//ダメージを質量と移動速度から計算
	damage = N * move.Length() * 0.005f;
}

void Player::Reflection()
{
	//移動適応
	object->SetPos(pos);
	object->SetScale(scale);
	object->SetRotation({ 0.0f,angle,0.0f });
	object->Update();

	frame->SetPos(object->GetPos());
	//frame->SetRotation(object->GetRotation());
	frame->SetScale(object->GetScale());
	frame->Update();
}

//描画
void Player::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	frame->Draw();
	Object3d::PostDraw();
}

//体あたり
void Player::Attack()
{
	static float attackCounter = 0.0f;	//攻撃後のアニメーションに使うカウンタ
	static float startScale = scale.x;		//スタート時のサイズ

	//体当たり貯め
	if (ControllerInput::GetPadButtonPress(XBOX_INPUT_A)) {
		if (hp < maxHp * 1.5) {
			hp += 0.1f;
		}
	}
	//体当たり開始
	if (ControllerInput::IsPadButtonReturn(XBOX_INPUT_A) ) {
		startScale = hp;						//体当たり開始時のhpを取得
		//attackAngle = rotate;
		atackSpeed = 2.0f;			//体当たりの初速決定
		hp *= 0.7f;
		attackCounter = 0.0f;		//カウンターを0に

		state = ATTACK;				//攻撃開始
	}

	//体当たり中処理
	if (state == ATTACK) {
		//attackAngle = rotate;
		//攻撃速度が0以下になった時攻撃を終了
		if (atackSpeed < 0.0f) {
			atackSpeed = 0.0f;
			state = STAY;	//攻撃終了
		}
		else {
			move += rotate * atackSpeed;
			atackSpeed -= 0.05f;

			if (attackCounter > 1.0f) {
				attackCounter = 1.0f;
				atackSpeed = 0.0f;
			}
			else {
				attackCounter += 0.02f;
			}

		}
	}
	scale = {
	(float)Ease(Out, Elastic, attackCounter, startScale ,hp),
	(float)Ease(Out, Elastic, attackCounter, startScale ,hp),
	(float)Ease(Out, Elastic, attackCounter, startScale ,hp)
	};
}

void Player::Damage(float damage)
{
	if (state == DAMAGE ||
		state == ATTACK) {
		return;
	}
	hp -= damage;
	scale = { hp,hp,hp };
	state = DAMAGE;
	returnDamageCount = 0;
}

void Player::LockOn(const vector<GameObjCommon *> gameObj)
{
	float minlength = 2000.0f;
	Vector3 target = { 0 ,0, 0 };
	for (int i = 0; i < gameObj.size(); i++) {
		if (!Collision::IsPredictCollisionBall(pos, move, r * 10, gameObj[i]->pos, gameObj[i]->move, gameObj[i]->r * 10)) {
			continue;
		}
		float ABlength = Vector3(gameObj[i]->pos - pos).Length();
		if (minlength > ABlength) {
			minlength = ABlength;
			target = gameObj[i]->pos;
		}
	}
	//更新されていればロックオン
	if (minlength < 2000.0f) {
		isLockOn = true;
		ChangeAngle(target - pos, 0.2f, Vector3(0, 0, 1));
	}
}

void Player::Hit()
{
	isHit = true;
	atackSpeed = 0.0f;
	returnHitCount = 0;
}



void Player::Heal()
{
	//ダメージ中なら復帰処理
	if (state == DAMAGE) {
		returnDamageCount++;
		if (returnDamageCount >= 30) {
			state = STAY;
		}
	}
	if (isHit) {
		returnHitCount++;
		if (returnHitCount > 10) {
			isHit = false;
		}
	}
	//待機中は回復
	if (hp < maxHp &&state == STAY) {
		hp += 0.01f;
	}
	if (hp > maxHp && state == STAY) {
		hp -= 0.01f;
	}
}


void Player::ChangeAngle(Vector3 targetPos, float ratio, Vector3 BaseAxis)
{
	float startAngle = object->GetRotation().y;	//開始時角度
	float endAngle = 0.0f;						//終了角度
	
	targetPos.Normalize();	//入力角度正規化
	endAngle = targetPos.VDot(BaseAxis);	//角度算出
	if (targetPos.x > 0) {
		endAngle = +acos(endAngle) * 180 / XM_PI;
	}
	else {
		endAngle = -acos(endAngle) * 180 / XM_PI;
	}
	//360度に直す
	startAngle = yMath::ChangeDegree360(startAngle);
	endAngle = yMath::ChangeDegree360(endAngle);
	//移動量が180を超える場合、初期位置を負数にする
	if (fabsf(endAngle - startAngle) > 180) {
		if (startAngle > 180) {
			startAngle -= 360;
		}
		else if (startAngle < 180) {
			startAngle += 360;
		}
	}
	//y軸の回転角計算
	angle = startAngle + (endAngle - startAngle) * ratio;
	rotate.AddRotationY(yMath::DegToRad(angle - startAngle));
}

bool Player::VectorMove()
{
	move += rotate * stickRotate.Length() * 0.0005f;
	return false;
}

bool Player::XYMove()
{
	move.x += stickRotate.x* 0.0005f;
	move.z += stickRotate.y* 0.0005f;
	return false;
}
