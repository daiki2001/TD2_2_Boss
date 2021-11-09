#include "Looper.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "AudioManager.h"	//音声管理
#include "ModelManager.h"

//シーン
#include "TitleScene.h"

Looper::Looper() {
	sceneStack.push(make_shared<TitleScene>(this));
	sceneStack.top()->Initialize();
	
	KeyboardInput::Initialize();		//入力初期化

}

bool Looper::Loop()
{
	KeyboardInput::Update();
	sceneStack.top()->Update();			//スタック更新

	sceneStack.top()->Draw();			//スタック描画
	DirectXCommon::PlayCommandList();	//描画コマンド実行

	if (KeyboardInput::GetKeyPressT(DIK_ESCAPE)) {
		return false;
	}
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
