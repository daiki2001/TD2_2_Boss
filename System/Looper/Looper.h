#pragma once
#include "AbstractScene.h"
#include "IoChangedListener.h"
#include <stack>
#include <memory>


using namespace std;

class Looper
	: public IoChangedListener {	
public:
	Looper();
	bool Loop();
	void OnSceneChanged(const Scenes scene, const bool stacClear) override;
	void SceneStackPop() override;	//ポップアウト

private:
	stack<shared_ptr<AbstractScene>> sceneStack;
};

