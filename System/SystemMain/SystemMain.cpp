#include "SystemMain.h"
#include "Looper.h"

#include "WinApp.h"			//ウィンドウの表示等
#include "DirectXCommon.h"	//DirectX初期化等
#include "Object3d.h"		//オブジェクト
#include "Object2d.h"
#include "ImageManager.h"
#include "AudioManager.h"
#include "ModelManager.h"			//モデル
#include "../CG2_01_01/Particle/ParticleManager.h" //パーティクル


bool SystemMain::Initialize() const
{
	WinApp::WindowCreate();				//ウィンドウ生成
	DirectXCommon::Initialize();		//DirectX初期化

	
	Object3d::StaticInitialize(							//オブジェクト共通データ生成
		DirectXCommon::dev.Get(), 
		WinApp::window_width, WinApp::window_height);
	Model::StaticInitialize(DirectXCommon::dev.Get());	//モデル姿勢的初期化

	
	Object2d::SpriteCommonCreate(WinApp::window_width, WinApp::window_height, DirectXCommon::dev.Get());	//スプライト共通データ生成
	ImageManager::GetIns()->LoadImagALL(DirectXCommon::dev.Get());				//画像読み込み
	AudioManager::Initialize();
	ModelManager::GetIns()->Initialize();			//モデル読み込み

	ParticleManager::StaticInitialize(DirectXCommon::dev.Get(), WinApp::window_width, WinApp::window_height); //パーティクル共通データ生成

	return true;
}

void SystemMain::Finalize() const
{
	AudioManager::SoundUnLoad(AudioManager::sound);		//音声を開放
	WinApp::WindowClassRelease();	// ウィンドウクラスを登録解除
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
