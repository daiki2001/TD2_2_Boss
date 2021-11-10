#include "Stage.h"
#include "ModelManager.h"
#include "DirectXCommon.h"

Stage::Stage()
{
	object = nullptr;
	object = Object3d::Create();
	object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Stage));
	object->Update();
	object->SetScale({ 10.0f,10.0f,10.0f });
	object->SetPos({ 0.0f, -20.0f, 0.0f });
	Initialize();
}

void Stage::Initialize()
{
	object->Initialize();
}

void Stage::Update()
{
	object->Update();
}

void Stage::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	Object3d::PostDraw();
	
}