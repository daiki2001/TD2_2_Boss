#include "Input.h"
#include "../DirectX/Input/KeyboardInput.h"
#include "../DirectX/Input/ControllerInput.h"

void Input::Init()
{
	KeyboardInput::Initialize();
	ControllerInput::Init();
}

void Input::Update()
{
	KeyboardInput::Update();
	ControllerInput::Update();
}

bool Input::Exit()
{
	if (KeyboardInput::GetKeyPressT(DIK_ESCAPE))
	{
		return true;
	}

	if (ControllerInput::IsPadButton(XBOX_INPUT_SELECT))
	{
		return true;
	}

	return false;
}

Vector3 Input::Move()
{
	static float x = 0.0f, y = 0.0f;
	x = 0.0f;
	y = 0.0f;

	if (KeyboardInput::GetKeyPress(DIK_W) || KeyboardInput::GetKeyPress(DIK_UP) ||
		KeyboardInput::GetKeyPress(DIK_A) || KeyboardInput::GetKeyPress(DIK_LEFT) ||
		KeyboardInput::GetKeyPress(DIK_S) || KeyboardInput::GetKeyPress(DIK_DOWN) ||
		KeyboardInput::GetKeyPress(DIK_D) || KeyboardInput::GetKeyPress(DIK_RIGHT))
	{
		if (KeyboardInput::GetKeyPress(DIK_A) || KeyboardInput::GetKeyPress(DIK_LEFT))
		{
			x -= 1000.0f;
		}
		if(KeyboardInput::GetKeyPress(DIK_D) || KeyboardInput::GetKeyPress(DIK_RIGHT))
		{
			x += 1000.0f;
		}
		if(KeyboardInput::GetKeyPress(DIK_W) || KeyboardInput::GetKeyPress(DIK_UP))
		{
			y += 1000.0f;
		}
		if(KeyboardInput::GetKeyPress(DIK_S) || KeyboardInput::GetKeyPress(DIK_DOWN))
		{
			y -= 1000.0f;
		}

		return Vector3(x, y, 0.0f);
	}

	x = (float)ControllerInput::IsPadStick(INPUT_AXIS_X, 0.2f);
	y = -(float)ControllerInput::IsPadStick(INPUT_AXIS_Y, 0.2f);

	return Vector3(x, y, 0.0f);
}

bool Input::AttackPrepare()
{
	static bool control = false;
	control = KeyboardInput::GetKeyPress(DIK_LCONTROL)
		|| KeyboardInput::GetKeyPress(DIK_RCONTROL);

	if (control)
	{
		return false;
	}

	if (KeyboardInput::GetKeyPress(DIK_SPACE))
	{
		return true;
	}

	if (ControllerInput::IsPadButton(XBOX_INPUT_A))
	{
		return true;
	}

	return false;
}

bool Input::Attack()
{
	static bool control = false;
	control = KeyboardInput::GetKeyPress(DIK_LCONTROL)
		|| KeyboardInput::GetKeyPress(DIK_RCONTROL);

	if (control)
	{
		return false;
	}

	static bool isFarst = true; //ÉQÅ[ÉÄäJéníºå„Ç©Ç«Ç§Ç©

	if (KeyboardInput::GetKeyReleasT(DIK_SPACE))
	{
		if (isFarst)
		{
			isFarst = false;
		}
		else
		{
			return true;
		}
	}

	if (ControllerInput::IsPadButtonReturn(XBOX_INPUT_A))
	{
		return true;
	}

	return false;
}

bool Input::LockOn()
{
	if (KeyboardInput::GetKeyPress(DIK_R))
	{
		return true;
	}

	if (ControllerInput::IsPadButton(XBOX_INPUT_RB))
	{
		return true;
	}

	return false;
}

bool Input::Test()
{
	static bool control = false;
	control = KeyboardInput::GetKeyPress(DIK_LCONTROL)
		|| KeyboardInput::GetKeyPress(DIK_RCONTROL);

	if (control && KeyboardInput::GetKeyPress(DIK_SPACE))
	{
		return true;
	}

	return false;
}
