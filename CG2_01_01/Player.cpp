#include "Player.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Easing.h"
#include "Collision.h"
#include "yMath.h"


Player::Player()
{
	object = nullptr;
	object = Object3d::Create();
	object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Player));
	object->SetPos({ 0.0f,10.0f,0.0f });
	object->Update();
	Initialize();
	
}

void Player::Initialize()
{
	r = 5.0f;
	hp = maxHp;
	pos = { 0.0f, 0.0f, 0.0f };
	scale = { hp,hp,hp };
	rotate = { 0.0f,0.0f,1.0f };
	moveSpeead = 0.0f;
	atackSpeed = 0.0f;
	state = STAY;
}

void Player::Update()
{
	
	//移動量初期化
	move = { 0.0f, 0.0f, 0.0f };

	Heal();			//自動回復
	ChangeAngle();	//角度変更
	Move();			//移動
	Attack();
	
	

	//移動適応
	move.y = 0.0f;	//yを無効化
	pos += move;

	object->Update();
}

void Player::Reflection()
{
	object->SetPos(pos);
	object->SetScale(scale);
	object->SetRotation({ 0.0f,angle,0.0f });
	object->Update();
}

//描画
void Player::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	Object3d::PostDraw();
}

//体あたり
void Player::Attack()
{
	static float attackCounter = 0.0f;	//攻撃後のアニメーションに使うカウンタ
	static float startScale = scale.x;		//スタート時のサイズ

	//体当たり
	if (ControllerInput::IsPadButtonReturn(XBOX_INPUT_A) && state == STAY) {
		startScale = hp;			//体当たり開始時のhpを取得
		//attackAngle = rotate;
		atackSpeed += 5.0f;			//体当たりの初速決定
		hp /= 2.0f;
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



void Player::Heal()
{
	if (hp < maxHp &&state == STAY) {
		hp += 0.01f;
	}
	else if (hp > 5.0f) {
		hp = maxHp;
	}
}


void Player::ChangeAngle()
{
	float startAngle = object->GetRotation().y;	//開始時角度
	float endAngle = 0.0f;						//終了角度
	Vector3 stickRotate = {						//現在の入力向きベクトル
		(float)ControllerInput::IsPadStick(INPUT_AXIS_X,0.2f),
		-(float)ControllerInput::IsPadStick(INPUT_AXIS_Y,0.2f),
		0.0f
	};
	//入力がある場合のみ
	if (stickRotate != Vector3{ 0.0f, 0.0f, 0.0f }) {
		stickRotate.Normalize();	//入力角度正規化
		endAngle = stickRotate.VDot(Vector3{ 0.0f,1.0f,0.0f });	//角度算出
		if (stickRotate.x > 0) {
			endAngle = +acos(endAngle) * 180 / XM_PI;
		}
		else {
			endAngle = -acos(endAngle) * 180 / XM_PI;
		}
		//360度に直す
		startAngle = yMath::ChangeDegree360(startAngle);
		endAngle = yMath::ChangeDegree360(endAngle);
		//移動量が180を超える場合、初期位置を負数にする
		if(fabsf(endAngle - startAngle) > 180){
			if (startAngle > 180) {
				startAngle -= 360;
			}
			else if (startAngle < 180) {
				startAngle += 360;
			}
		}
		//y軸の回転角計算
		angle = startAngle + (endAngle - startAngle) / 20.0f;	
		rotate.AddRotationY(yMath::DegToRad(angle - startAngle));
	}
}

bool Player::Move()
{
	Vector3 stickRotate = {						//現在の入力向きベクトル
		(float)ControllerInput::IsPadStick(INPUT_AXIS_X,0.2f),
		-(float)ControllerInput::IsPadStick(INPUT_AXIS_Y,0.2f),
		0.0f
	};

	move += rotate * stickRotate.Length() / 800 * 2;
	return false;
}
