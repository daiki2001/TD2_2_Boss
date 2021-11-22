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
	//�t���[��
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
	//���͂��ŐV��
	stickRotate = {						//���݂̓��͌����x�N�g��
		(float)ControllerInput::IsPadStick(INPUT_AXIS_X,0.2f),
		-(float)ControllerInput::IsPadStick(INPUT_AXIS_Y,0.2f),
		0.0f
	};
	//�ړ��ʏ�����
	if(move.Length() > 0.1f){
		move = move * 0.95f;
	}
	else {
		move = { 0,0,0 };
	}
	
	Heal();			//������
	//���b�N�I�������Ɉړ����Ă���ꍇ�X�V
	if (stickRotate != Vector3{ 0.0f, 0.0f, 0.0f } && !isLockOn){
		ChangeAngle(stickRotate, 0.1f, Vector3(0, 1, 0));	//�p�x�ύX
	}
	//�ڐG��d�����Ȃ���Έړ��ƍU���\
	if(!isHit){
		if(isLockOn){
			XYMove();
		}
		else {
			VectorMove();			//�ړ�
		}
	}
	Attack();



	//�ړ��K��
	move.y = 0.0f;	//y�𖳌���
	pos += move;
	//HP�����ʂɓK��
	N = hp * 5;
	r = hp * 1.5f;

	//�_���[�W�����ʂƈړ����x����v�Z
	damage = N * move.Length() * 0.005f;
}

void Player::Reflection()
{
	//�ړ��K��
	object->SetPos(pos);
	object->SetScale(scale);
	object->SetRotation({ 0.0f,angle,0.0f });
	object->Update();

	frame->SetPos(object->GetPos());
	//frame->SetRotation(object->GetRotation());
	frame->SetScale(object->GetScale());
	frame->Update();
}

//�`��
void Player::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	frame->Draw();
	Object3d::PostDraw();
}

//�̂�����
void Player::Attack()
{
	static float attackCounter = 0.0f;	//�U����̃A�j���[�V�����Ɏg���J�E���^
	static float startScale = scale.x;		//�X�^�[�g���̃T�C�Y

	//�̓����蒙��
	if (ControllerInput::GetPadButtonPress(XBOX_INPUT_A)) {
		if (hp < maxHp * 1.5) {
			hp += 0.1f;
		}
	}
	//�̓�����J�n
	if (ControllerInput::IsPadButtonReturn(XBOX_INPUT_A) ) {
		startScale = hp;						//�̓�����J�n����hp���擾
		//attackAngle = rotate;
		atackSpeed = 2.0f;			//�̓�����̏�������
		hp *= 0.7f;
		attackCounter = 0.0f;		//�J�E���^�[��0��

		state = ATTACK;				//�U���J�n
	}

	//�̓����蒆����
	if (state == ATTACK) {
		//attackAngle = rotate;
		//�U�����x��0�ȉ��ɂȂ������U�����I��
		if (atackSpeed < 0.0f) {
			atackSpeed = 0.0f;
			state = STAY;	//�U���I��
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
	//�X�V����Ă���΃��b�N�I��
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
	//�_���[�W���Ȃ畜�A����
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
	//�ҋ@���͉�
	if (hp < maxHp &&state == STAY) {
		hp += 0.01f;
	}
	if (hp > maxHp && state == STAY) {
		hp -= 0.01f;
	}
}


void Player::ChangeAngle(Vector3 targetPos, float ratio, Vector3 BaseAxis)
{
	float startAngle = object->GetRotation().y;	//�J�n���p�x
	float endAngle = 0.0f;						//�I���p�x
	
	targetPos.Normalize();	//���͊p�x���K��
	endAngle = targetPos.VDot(BaseAxis);	//�p�x�Z�o
	if (targetPos.x > 0) {
		endAngle = +acos(endAngle) * 180 / XM_PI;
	}
	else {
		endAngle = -acos(endAngle) * 180 / XM_PI;
	}
	//360�x�ɒ���
	startAngle = yMath::ChangeDegree360(startAngle);
	endAngle = yMath::ChangeDegree360(endAngle);
	//�ړ��ʂ�180�𒴂���ꍇ�A�����ʒu�𕉐��ɂ���
	if (fabsf(endAngle - startAngle) > 180) {
		if (startAngle > 180) {
			startAngle -= 360;
		}
		else if (startAngle < 180) {
			startAngle += 360;
		}
	}
	//y���̉�]�p�v�Z
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
