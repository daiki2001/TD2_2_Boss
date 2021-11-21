#include "Boss.h"
#include "DirectXCommon.h"

int Boss::stayTimer = 120;

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
	counter = 0;
}

void Boss::Initialize()
{
	move = { 0,0,0 };
}

void Boss::Update()
{

	//�e�X�g�p
	counter++;


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
	//�ҋ@��Ԃ��^�C�}�[��0�Ȃ�s������
	if (state == Stay && stayTimer == 0) {
		AttackSelect();
	}
	if (state == ATTACK) {

		Vector3 playerVec = playerData->pos - pos;
		playerVec.Normalize();			//�v���C���[�̌����x�N�g��
		switch (atackState)
		{
		case Boss::Stay:
			break;
		case Boss::Short:
			move += playerVec * 10;
			oldAtackState = Short;
			break;
		case Boss::Middle:
			move += playerVec * 15;
			oldAtackState = Middle;
			break;
		case Boss::Long:
			move += playerVec * 20;
			oldAtackState = Long;
			break;
		default:
			break;
		}
		atackState = Stay;
		state = STAY;
	}

	//�ړ��K��
	pos += move;
	N = hp * 5;
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
	object->SetPos(pos);
	object->Update();
	frame->SetRotation(spinFrame());
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
	if (ChackRange(500, 0)) {
		atackState = Short;			//���̍U���p�^�[��������
		stayTimer = 120;			//�U���I����̃^�C�}�[�Z�b�g
	}
	else if (ChackRange(700, 500)) {
		atackState = Middle;		//���̍U���p�^�[��������
		stayTimer = 180;			//�U���I����̃^�C�}�[�Z�b�g
	}
	else if (ChackRange(1000, 700)) {
		atackState = Long;			//���̍U���p�^�[��������
		stayTimer = 240;			//�U���I����̃^�C�}�[�Z�b�g
	}
	else {
		//�Y�������̊O���ɂ���Ƃ��͍s�����Ȃ�
		atackState = Stay;
		state = STAY;
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

Vector3 Boss::spinFrame()
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
