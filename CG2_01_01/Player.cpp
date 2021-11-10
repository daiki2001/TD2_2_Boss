#include "Player.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "ModelManager.h"
#include "DirectXCommon.h"
#include "Easing.h"
#include "Collision.h"

Player::Player()
{
	object = nullptr;
	object = Object3d::Create();
	object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Player));
	object->SetPos({ 0.0f,10.0f,0.0f });
	object->Update();
	moveSpeead = 0.0f;
	atackSpeed = 0.0f;
	Initialize();
}

void Player::Initialize()
{
	pos = { 0.0f, 0.0f, 0.0f };
	scale = { 1.0f,1.0f,1.0f };
	rotate = { 0.0f,0.0f,0.0f };
	energy = 1.0f;

	isAttack = false;
}

void Player::Update()
{
	move = { 0.0f, 0.0f, 0.0f };

	
	Attack();
	Move();
	
	

	//移動適応
	move.y = 0.0f;	//yを無効化
	pos += move;

	//モデルの更新

	if ((float)ControllerInput::IsPadStick(INPUT_AXIS_X, 0.2f) != 0 ||
		(float)ControllerInput::IsPadStick(INPUT_AXIS_Y, 0.2f) != 0) {
		ChangeAngle();				//角度変更
	}
	object->SetPos(pos);
	object->SetScale(scale);
	object->Update();
}

void Player::Draw() const
{

	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	Object3d::PostDraw();
}

void Player::Attack()
{
	static float attackCounter = 0.0f;	//攻撃イージング
	static float startScale = 0.0f;
	//デバック用エネルギー補充
	if (ControllerInput::IsPadButtonTrigger(XBOX_INPUT_B)) {
		energy += 0.1f;
	}
	//体当たり
	if (ControllerInput::IsPadButtonTrigger(XBOX_INPUT_A) && !isAttack) {
		startScale = energy;
		atackSpeed += energy / 2.0f * 5.0f;
		energy /= 2.0f;
		attackCounter = 0.0f;		//カウンターを0に

		isAttack = true;	//攻撃開始
	}


	if (atackSpeed < 0.0f) {
		atackSpeed = 0.0f;
		isAttack = false;	//攻撃終了
	}
	else {
		move.x += rotate.x * atackSpeed;
		move.z += rotate.y * atackSpeed;
		atackSpeed -= 0.05f;



		if (attackCounter > 1.0f) {
			attackCounter = 1.0f;
		}
		else {
			scale = {
			(float)Ease(Out, Elastic, attackCounter, startScale ,energy),
			(float)Ease(Out, Elastic, attackCounter, startScale ,energy),
			(float)Ease(Out, Elastic, attackCounter, startScale ,energy)
			};

			attackCounter += 0.02f;
		}

	}
}


bool Player::Move()
{
	move.x += (float)ControllerInput::IsPadStick(INPUT_AXIS_X, 0.2f) / 1000;
	move.z += -(float)ControllerInput::IsPadStick(INPUT_AXIS_Y, 0.2f) / 1000;

	return false;
}

void Player::ChangeAngle()
{
	rotate = {
		(float)ControllerInput::IsPadStick(INPUT_AXIS_X, 0.2f) ,
		-(float)ControllerInput::IsPadStick(INPUT_AXIS_Y, 0.2f) ,
		0.0f
	};
	rotate.Normalize();
	float angle = rotate.VDot(Vector3{ 0.0f,1.0f,0.0f });
	if(move.x < 0){
		angle = -acos(angle) * 180 / XM_PI;
	}
	else {
		angle = acos(angle) * 180 / XM_PI;
	}
	object->SetRotation({ 0.0f,angle,0.0f });

}
