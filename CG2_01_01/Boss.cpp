#include "Boss.h"
#include "DirectXCommon.h"

Boss::Boss(Player *player, Vector3 startPos, float hp, float N, float e) :
	BaseEnemy(player, startPos, hp, N, e, ModelManager::ModelName::BossCore) {

	frame = nullptr;
	frame = Object3d::Create();
	frame->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BossFrame));
	frame->SetPos(object->GetPos());
	frame->SetRotation(object->GetRotation());
	frame->SetScale(object->GetScale());
	frame->Initialize();
	frame->Update();
}

void Boss::Initialize()
{

}

void Boss::Update()
{
}

void Boss::Reflection()
{
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
