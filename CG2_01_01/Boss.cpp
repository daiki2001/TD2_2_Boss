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
	//�v���C���[�̌����x�N�g��
	playerVec = playerData->pos - pos;
	playerVec.Normalize();


	//���S����
	if (!isAlive) {
		Dead();
	}

	//�ړ��ʏ�����
	if (move.Length() > 0.1f) {
		move = move * 0.95f;
	}
	else {
		move = { 0,0,0 };
	}

	//�ҋ@��ԂŃ^�C�}�[���c���Ă���Ό���������
	if (state == Stay && stayTimer > 0) {
		stayTimer--;
	}
	//�ҋ@��ԂȂ�U�������肷��
	if (state == Stay && stayTimer == 0) {
		if (nextAttackType == Stay) {
			AttackSelect();
		}
		else {
			state = ATTACK;
		}
	}
	if (state == ATTACK) {

		//�U���O���[�V�������I�������U���J�n
		if (StandbyMotion()) {
			switch (attackType)
			{
			case Boss::Stay:
				state = STAY;
				break;
				//�ʏ�̃^�b�N��
			case Boss::Tackle:
				stayTimer = 120;			//�U���I����̃^�C�}�[�Z�b�g
				move += playerVec * 40;
				oldAtackType = Tackle;
				if(hp < 20){
					nextAttackType = Tackle3;
				}
				break;
			
				//�O�A�^�b�N���iHP20�ȉ��j
			case Boss::Tackle3:
				stayTimer = 30;			//�U���I����̃^�C�}�[�Z�b�g
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

	//�ړ��K��
	pos += move;
	N = hp * 3;
	r = hp * 1.5f;
	//�_���[�W�����ʂƈړ����x����v�Z
	damage = N * move.Length() * 0.005f;

	//�t���[���̉�]
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
{		//�����ɉ����čU�����@��I��
	state = ATTACK;
	if (ChackRange(100, 0)) {
		//attackType = Tackle;			//���̍U���p�^�[��������
	}
	else if (ChackRange(700, 450)) {
		attackType = Tackle;			//���̍U���p�^�[��������
	}
	else if (ChackRange(1000, 1000)) {
		//attackType = Long;			//���̍U���p�^�[��������
		//stayTimer = 240;			//�U���I����̃^�C�}�[�Z�b�g
	}
	else {
		//�Y�������̊O���ɂ���Ƃ��͍s�����Ȃ�
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
		//��]�ʑ���
		frameRotate.y += Ease(In, Back, (float)standEaseTimer / 30.0f, 0.0f, 15.0f);
		//���
		if (standMotionTimer == 15) {
			move -= playerVec * 10.0f;
		}
		//��]
		if (standEaseTimer >= 30) {
			standEaseTimer = 30;
		}
		//�U���J�n
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