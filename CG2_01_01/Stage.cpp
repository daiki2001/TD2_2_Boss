#include "Stage.h"
#include "ModelManager.h"
#include "DirectXCommon.h"

Stage::Stage()
{
	floor = nullptr;
	floor = Object3d::Create();
	floor->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Stage));
	floor->SetScale({8.0f,8.0f,8.0f });
	floor->SetRotation({ 0.0f,90.0f,0.0f });
	floor->SetPos({ 0.0f, -150.0f, 0.0f });
	floor->Update();


	wall = nullptr;
	wall = Object3d::Create();
	wall->SetModel(ModelManager::GetIns()->GetModel(ModelManager::StageWall));
	wall->SetParent(floor);
	wall->SetScale({ 10.0f ,10.0f ,10.0f });
	wall->Update();

	Initialize();
}

void Stage::Initialize()
{
	floor->Initialize();
	wall->Initialize();
}

void Stage::Update()
{
	floor->Update();
	wall->Update();

}

void Stage::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	floor->Draw();
	wall->Draw();
	Object3d::PostDraw();
	
}