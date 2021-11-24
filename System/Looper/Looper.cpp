#include "Looper.h"
#include "DirectXCommon.h"
#include "../CG2_01_01/Input.h" //入力
#include "AudioManager.h"	//音声管理
#include "ModelManager.h"

//シーン
#include "TitleScene.h"
#include "TestScene.h"

Looper::Looper() {
	sceneStack.push(make_shared<TestScene>(this));
	sceneStack.top()->Initialize();
	
	Input::Init();		//入力初期化

}

bool Looper::Loop()
{
	Input::Update();			//入力アップデート

	sceneStack.top()->Update();			//スタック更新

	sceneStack.top()->Draw();			//スタック描画
	DirectXCommon::PlayCommandList();	//描画コマンド実行

	if (Input::Exit())
	{
		return false;
	}

	//if (ControllerInput::IsPadButton(XBOX_INPUT_RIGHT))
	//{
	//	return false;
	//}

	return true;
}

void Looper::OnSceneChanged(const Scenes scene, const bool stackClear)
{
	if (stackClear == true) {				//スタックをクリアする設定なら
		while (!sceneStack.empty()) {	//スタックがからになるまでポップする
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
		//存在しないシーンがスタックトップにある際のエラー処理
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
