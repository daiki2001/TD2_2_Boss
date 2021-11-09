#include "Looper.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "AudioManager.h"	//�����Ǘ�
#include "ModelManager.h"
#include "ControllerInput.h"

//�V�[��
#include "TitleScene.h"

Looper::Looper() {
	sceneStack.push(make_shared<TitleScene>(this));
	sceneStack.top()->Initialize();
	
	KeyboardInput::Initialize();		//���͏�����
	ControllerInput::Init();

}

bool Looper::Loop()
{
	KeyboardInput::Update();
	ControllerInput::Update();
	sceneStack.top()->Update();			//�X�^�b�N�X�V

	sceneStack.top()->Draw();			//�X�^�b�N�`��
	DirectXCommon::PlayCommandList();	//�`��R�}���h���s

	if (KeyboardInput::GetKeyPressT(DIK_ESCAPE)) {
		return false;
	}

	if (ControllerInput::IsPadButton(XBOX_INPUT_SELECT))
	{
		return false;
	}

	return true;
}

void Looper::OnSceneChanged(const Scenes scene, const bool stackClear)
{
	if (stackClear == true) {				//�X�^�b�N���N���A����ݒ�Ȃ�
		while (!sceneStack.empty()) {	//�X�^�b�N������ɂȂ�܂Ń|�b�v����
			sceneStack.pop();
		}
	}
	switch (scene) {
	case Title:
		sceneStack.push(make_shared<TitleScene>(this));
		break;
	case Test:
		break;
	
	default:
		//���݂��Ȃ��V�[�����X�^�b�N�g�b�v�ɂ���ۂ̃G���[����
		break;
	}
	sceneStack.top()->Initialize();
}

void Looper::SceneStackPop()
{
	sceneStack.top()->Finalize();
	sceneStack.pop();
	sceneStack.top()->Initialize();
}
