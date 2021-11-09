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
	title = Object2d::SpriteCreate(DirectXCommon::dev.Get(), WinApp::window_width, WinApp::window_height, ImageManager::GetIns()->BackGround);
	title.position.x = WinApp::window_width /2;
	title.position.y = WinApp::window_height/2;

	obj1 = Object3d::Create();
	obj1->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Player));
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
	if (KeyboardInput::GetKeyPress(DIK_SPACE)) {
		//implSceneChanged->OnSceneChanged(Test);
		//AudioManager::SoundStopWave(AudioManager::GameBgm);
		obj1->rotation.z++;
	}

	// ���X�e�B�b�NX��
	if (ControllerInput::IsPadStick(INPUT_AXIS_X, 0.2f))
	{
		obj1->rotation.x++;
	}
	// ���X�e�B�b�NY��
	if (ControllerInput::IsPadStick(INPUT_AXIS_Y, 0.2f))
	{
		obj1->rotation.y++;
	}
	// �E�X�e�B�b�NX��
	if (ControllerInput::IsPadStick(INPUT_AXIS_RX, 0.2f))
	{
		obj2->rotation.x++;
	}
	// �E�X�e�B�b�NY��
	if (ControllerInput::IsPadStick(INPUT_AXIS_RY, 0.2f))
	{
		obj2->rotation.y++;
	}

	Object2d::SpriteUpdate(title);
	obj1->Update();
	obj2->Update();

	player.Update();
}

void TitleScene::Draw() const
{
	Object2d::SpriteCommonBeginDraw(DirectXCommon::cmdList.Get());	//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
	Object2d::SpriteDraw(title, DirectXCommon::cmdList.Get(), DirectXCommon::dev.Get());


	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	obj1->Draw();
	obj2->Draw();
	Object3d::PostDraw();

	player.Draw();

}
