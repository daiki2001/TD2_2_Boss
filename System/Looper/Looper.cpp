#include "Looper.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"	//�L�[�{�[�h
#include "ControllerInput.h"//�R���g���[���[
#include "AudioManager.h"	//�����Ǘ�
#include "ModelManager.h"

//�V�[��
#include "TitleScene.h"
#include "TestScene.h"

Looper::Looper() {
	sceneStack.push(make_shared<TestScene>(this));
	sceneStack.top()->Initialize();
	
	KeyboardInput::Initialize();		//���͏�����
	ControllerInput::Init();
}

bool Looper::Loop()
{
	KeyboardInput::Update();			//�L�[�{�[�h�A�b�v�f�[�g
	ControllerInput::Update();			//�R���g���[���A�b�v�f�[�g
	sceneStack.top()->Update();			//�X�^�b�N�X�V

	sceneStack.top()->Draw();			//�X�^�b�N�`��
	DirectXCommon::PlayCommandList();	//�`��R�}���h���s

	if (KeyboardInput::GetKeyPressT(DIK_ESCAPE) ||
		ControllerInput::IsPadButton(XBOX_INPUT_SELECT)) {
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
		sceneStack.push(make_shared<TestScene>(this));
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
