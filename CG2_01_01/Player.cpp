#include "Player.h"
#include "KeyboardInput.h"
#include "ModelManager.h"
#include "DirectXCommon.h"

Player::Player()
{
	object = nullptr;
	object = Object3d::Create();
	object->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Player));
	object->Update();
	Initialize();
}

void Player::Initialize()
{
	pos = { 0.0f, 0.0f, 0.0f };
}

void Player::Update()
{
	move = { 0.0f, 0.0f, 0.0f };
	if (KeyboardInput::GetKeyPress(DIK_A)) {
		move.x = -1.0f;
	}

	pos += move;

	object->SetPos(pos);
	object->Update();
}

void Player::Draw() const
{

	Object3d::PreDraw(DirectXCommon::cmdList.Get());
	object->Draw();
	Object3d::PostDraw();
}
