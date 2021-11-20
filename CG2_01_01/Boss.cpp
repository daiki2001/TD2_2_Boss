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

}

void Boss::Update()
{
	//テスト用
	counter++;
	AttackSelect();
}

void Boss::Reflection()
{
	object->SetPos(pos);
	object->Update();
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
		//距離に応じて攻撃方法を選択
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
