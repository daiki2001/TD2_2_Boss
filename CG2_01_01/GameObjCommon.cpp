#include "GameObjCommon.h"
#include "DirectXCommon.h"

void GameObjCommon::Initialize()
{
}

void GameObjCommon::Update()
{
	Reflection();
}

void GameObjCommon::Reflection()
{
	object->SetScale(scale);
	object->SetPos(pos);
	object->SetRotation(rotate);

	object->Update();
}

void GameObjCommon::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	Object3d::PostDraw();
}

void GameObjCommon::Damage(float damage)
{
	hp -= damage;
	if (hp <= 5.0f) {
		hp = 5.0f;
		isAlive = false;
	}
}

void GameObjCommon::Dead()
{
}
