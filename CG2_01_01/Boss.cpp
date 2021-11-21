#include "Boss.h"
#include "DirectXCommon.h"

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
	AttackSelect();

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
{
	if (state == STAY && counter % 120 == 0) {
		//�����ɉ����čU�����@��I��
		if (ChackRange(300, 0)) {
			atackState = Short;
		}
		if (ChackRange(600, 300)) {
			atackState = Middle;
		}
		if (ChackRange(1000, 600)) {
			atackState = Long;
		}
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
