#include "SystemMain.h"
#include "Looper.h"

#include "WinApp.h"			//�E�B���h�E�̕\����
#include "DirectXCommon.h"	//DirectX��������
#include "Object3d.h"		//�I�u�W�F�N�g
#include "Object2d.h"
#include "ImageManager.h"
#include "AudioManager.h"
#include "ModelManager.h"			//���f��
#include "../CG2_01_01/Particle/ParticleManager.h" //�p�[�e�B�N��


bool SystemMain::Initialize() const
{
	WinApp::WindowCreate();				//�E�B���h�E����
	DirectXCommon::Initialize();		//DirectX������

	
	Object3d::StaticInitialize(							//�I�u�W�F�N�g���ʃf�[�^����
		DirectXCommon::dev.Get(), 
		WinApp::window_width, WinApp::window_height);
	Model::StaticInitialize(DirectXCommon::dev.Get());	//���f���p���I������

	
	Object2d::SpriteCommonCreate(WinApp::window_width, WinApp::window_height, DirectXCommon::dev.Get());	//�X�v���C�g���ʃf�[�^����
	ImageManager::GetIns()->LoadImagALL(DirectXCommon::dev.Get());				//�摜�ǂݍ���
	AudioManager::Initialize();
	ModelManager::GetIns()->Initialize();			//���f���ǂݍ���

	ParticleManager::StaticInitialize(DirectXCommon::dev.Get(), WinApp::window_width, WinApp::window_height); //�p�[�e�B�N�����ʃf�[�^����

	return true;
}

void SystemMain::Finalize() const
{
	AudioManager::SoundUnLoad(AudioManager::sound);		//�������J��
	WinApp::WindowClassRelease();	// �E�B���h�E�N���X��o�^����
}

void SystemMain::main() const
{
	Looper looper;
	while (DirectXCommon::ResourceBarrierWriting() && DirectXCommon::ScreenClear() && !!WinApp::WindowsMessage()) {
		if (!looper.Loop()) {
			break;
		}
	}
}
