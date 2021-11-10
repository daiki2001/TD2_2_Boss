#include "TestScene.h"
#include "Object3d.h"

TestScene::TestScene(IoChangedListener *impl)
	: AbstractScene(impl)
{
	stage.Initialize();
	player.Initialize();
}

void TestScene::Initialize()
{
	stage.Initialize();
	player.Initialize();
}

void TestScene::Finalize()
{
}

void TestScene::Update()
{
	stage.Update();
	player.Update();
	Object3d::SetCamPos({ player.pos.x - 80.0f,200,player.pos.z-50.0f });
	Object3d::SetCamTarget({ player.pos.x - 80.0f,0.0f,player.pos.z});

	Object3d::UpdateViewMatrix();
}

void TestScene::Draw() const
{
	stage.Draw();
	player.Draw();
}
