#include "TitleScene.h"
#include "KeyboardInput.h"
#include "ModelManager.h"
#include "ImageManager.h"
#include "DirectXCommon.h"
#include "Scene.h"
#include "AudioManager.h"
#include "ControllerInput.h"


TitleScene::TitleScene(IoChangedListener *impl)
	: AbstractScene(impl)
{
	
	obj1 = Object3d::Create();
	obj1->SetModel(ModelManager::GetIns()->GetModel(ModelManager::PlayerFrame));
	obj1->SetScale({ 2.0f,2.0f,2.0f });
	obj1->Update();
	obj1->Initialize();


	obj2 = Object3d::Create();
	obj2->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Triangle));
	obj2->SetScale({10.0f,10.0f,10.0f });
	obj2->SetPos({ 10.0f,0.0f,0.0f });
	obj2->Update();
	obj2->Initialize();


	Initialize();

	AudioManager::SoundPlayWave(AudioManager::GameBgm, true);
}

void TitleScene::Initialize()
{
	player.Initialize();
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Vector3 rot = obj1->GetRotation();

	if (KeyboardInput::GetKeyPress(DIK_SPACE)) {
		//implSceneChanged->OnSceneChanged(Test);
		//AudioManager::SoundStopWave(AudioManager::GameBgm);
		obj1->SetRotation(rot + Vector3(0.0f, 0.0f, 1.0f));
	}

	// 左スティックX軸
	if (ControllerInput::IsPadStick(INPUT_AXIS_X, 0.2f))
	{
		obj1->SetRotation(rot + Vector3(1.0f, 0.0f, 0.0f));
	}
	// 左スティックY軸
	if (ControllerInput::IsPadStick(INPUT_AXIS_Y, 0.2f))
	{
		obj1->SetRotation(rot + Vector3(0.0f, 20.0f, 0.0f));
	}
	// 右スティックX軸
	if (ControllerInput::IsPadStick(INPUT_AXIS_RX, 0.2f))
	{
		obj2->SetRotation(rot + Vector3(1.0f, 0.0f, 0.0f));
	}
	// 右スティックY軸
	if (ControllerInput::IsPadStick(INPUT_AXIS_RY, 0.2f))
	{
		obj2->SetRotation(rot + Vector3(0.0f, 1.0f, 0.0f));
	}

	obj1->Update();
	obj2->Update();

	player.Update();
}

void TitleScene::Draw() const
{

	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	obj1->Draw();
	obj2->Draw();
	Object3d::PostDraw();

	player.Draw();

}
