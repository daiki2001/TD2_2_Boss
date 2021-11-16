#include "BaseEnemy.h"
#include "DirectXCommon.h"

void BaseEnemy::Initialize()
{
}

void BaseEnemy::Update()
{
}

void BaseEnemy::Reflection()
{
	object->SetPos(pos);
	object->SetScale(scale);
	object->SetRotation({ 0.0f,angle,0.0f });
	object->Update();
}

void BaseEnemy::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	Object3d::PostDraw();
}
