#include "BaseEnemy.h"
#include "DirectXCommon.h"
#include "Easing.h"
void BaseEnemy::Initialize()
{
}

void BaseEnemy::Update()
{
}

void BaseEnemy::Reflection()
{
	object->SetPos(pos);
	object->SetScale(scale);
	object->SetRotation({ 0.0f,angle,0.0f });
	object->Update();
}

void BaseEnemy::Draw() const
{
	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	Object3d::PostDraw();
}


void BaseEnemy::Dead()
{

	//€–S‰‰o
	if (deadCounter < 1.0f) {
		deadCounter += 0.05f;

		scale = {
			(float)Ease(Out,Back,deadCounter,1.0f,10.0f),
			(float)Ease(Out,Back,deadCounter,1.0f,10.0f),
			(float)Ease(Out,Back,deadCounter,1.0f,10.0f),
		};
	}
	else {
		deadCounter += 0.08f;
		scale = {
			(float)Ease(Out,Back,deadCounter - 1.0f,5.0f,0.0f),
			(float)Ease(Out,Back,deadCounter - 1.0f,5.0f,0.0f),
			(float)Ease(Out,Back,deadCounter - 1.0f,5.0f,0.0f),
		};
		if (deadCounter >= 2.0f) {
			deadCounter = 2.0f;
			isDelete = true;
		}
	}
}
