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
	
	//�ړ��ʏ�����
	move = { 0.0f, 0.0f, 0.0f };

	Heal();			//������
	ChangeAngle();	//�p�x�ύX
	Move();			//�ړ�
	Attack();
	
	

	//�ړ��K��
	move.y = 0.0f;	//y�𖳌���
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

//�`��
void Player::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	Object3d::PostDraw();
}

//�̂�����
void Player::Attack()
{
	static float attackCounter = 0.0f;	//�U����̃A�j���[�V�����Ɏg���J�E���^
	static float startScale = scale.x;		//�X�^�[�g���̃T�C�Y

	//�̓�����
	if (ControllerInput::IsPadButtonReturn(XBOX_INPUT_A) && state == STAY) {
		startScale = hp;			//�̓�����J�n����hp���擾
		//attackAngle = rotate;
		atackSpeed += 5.0f;			//�̓�����̏�������
		hp /= 2.0f;
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
	float startAngle = object->GetRotation().y;	//�J�n���p�x
	float endAngle = 0.0f;						//�I���p�x
	Vector3 stickRotate = {						//���݂̓��͌����x�N�g��
		(float)ControllerInput::IsPadStick(INPUT_AXIS_X,0.2f),
		-(float)ControllerInput::IsPadStick(INPUT_AXIS_Y,0.2f),
		0.0f
	};
	//���͂�����ꍇ�̂�
	if (stickRotate != Vector3{ 0.0f, 0.0f, 0.0f }) {
		stickRotate.Normalize();	//���͊p�x���K��
		endAngle = stickRotate.VDot(Vector3{ 0.0f,1.0f,0.0f });	//�p�x�Z�o
		if (stickRotate.x > 0) {
			endAngle = +acos(endAngle) * 180 / XM_PI;
		}
		else {
			endAngle = -acos(endAngle) * 180 / XM_PI;
		}
		//360�x�ɒ���
		startAngle = yMath::ChangeDegree360(startAngle);
		endAngle = yMath::ChangeDegree360(endAngle);
		//�ړ��ʂ�180�𒴂���ꍇ�A�����ʒu�𕉐��ɂ���
		if(fabsf(endAngle - startAngle) > 180){
			if (startAngle > 180) {
				startAngle -= 360;
			}
			else if (startAngle < 180) {
				startAngle += 360;
			}
		}
		//y���̉�]�p�v�Z
		angle = startAngle + (endAngle - startAngle) / 20.0f;	
		rotate.AddRotationY(yMath::DegToRad(angle - startAngle));
	}
}

bool Player::Move()
{
	Vector3 stickRotate = {						//���݂̓��͌����x�N�g��
		(float)ControllerInput::IsPadStick(INPUT_AXIS_X,0.2f),
		-(float)ControllerInput::IsPadStick(INPUT_AXIS_Y,0.2f),
		0.0f
	};

	move += rotate * stickRotate.Length() / 800 * 2;
	return false;
}
