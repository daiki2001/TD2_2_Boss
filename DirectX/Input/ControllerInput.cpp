#include "ControllerInput.h"

const LONG ControllerInput::zoneMax = +1000;
const LONG ControllerInput::zoneMin = -1000;
IDirectInput8* ControllerInput::dinput = nullptr;
ControllerInput::vector<IDirectInputDevice8*> ControllerInput::devGamepad = {};
ControllerInput::vector<DIJOYSTATE> ControllerInput::gamepad = {};
ControllerInput::vector<DIJOYSTATE> ControllerInput::oldgamepad = {};

ControllerInput* ControllerInput::GetInstance()
{
	static ControllerInput instance;

	return &instance;
}

void ControllerInput::Init()
{
	HRESULT hr;

	hr = DirectInput8Create(
		WinApp::GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	hr = CreateGamePadDevice();
}

void ControllerInput::Update()
{
	HRESULT hr;

	if (devGamepad.size() == 0)
	{
		hr = CreateGamePadDevice();
		if (devGamepad.size() == 0)
		{
			return;
		}
	}

	for (size_t i = 0; i < devGamepad.size(); i++)
	{
		hr = devGamepad[i]->Acquire();
		if (hr == DIERR_INPUTLOST)
		{
			// デバイスがロストしていたら無視する
			continue;
		}

		devGamepad[i]->Poll();

		if (gamepad.size() < devGamepad.size())
		{
			do
			{
				// 数合わせ
				gamepad.push_back({});
				oldgamepad.push_back({});
			} while (gamepad.size() != devGamepad.size());
		}

		oldgamepad[i] = gamepad[i];

		ZeroMemory(&gamepad[i], sizeof(DIJOYSTATE));
		devGamepad[i]->GetDeviceState(sizeof(DIJOYSTATE), &gamepad[i]);
	}
}

DIJOYSTATE& ControllerInput::GetGamePadState(const size_t& gamePadNo)
{
	if (gamePadNo >= gamepad.size())
	{
		return DIJOYSTATE();
	}

	return gamepad[gamePadNo];
}

bool ControllerInput::IsPadButton(const size_t& button, const size_t& gamepadNo)
{
	if (gamepadNo >= gamepad.size())
	{
		return false;
	}
	if (button >= sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]) + 4)
	{
		return false;
	}

	if (button < sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]))
	{
		return gamepad[gamepadNo].rgbButtons[button];
	}
	else if (button == XBOX_INPUT_LEFT)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 5 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 6 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 7;

		return now;
	}
	else if (button == XBOX_INPUT_RIGHT)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 1 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 2 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 3;

		return now;
	}
	else if (button == XBOX_INPUT_UP)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 7 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 0 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 1;

		return now;
	}
	else if (button == XBOX_INPUT_DOWN)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 3 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 4 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 5;

		return now;
	}
}

bool ControllerInput::IsPadButtonTrigger(const size_t& button, const size_t& gamepadNo)
{
	if (gamepadNo >= gamepad.size())
	{
		return false;
	}
	if (button >= sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]) + 4)
	{
		return false;
	}

	if (button < sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]))
	{
		return gamepad[gamepadNo].rgbButtons[button] && !oldgamepad[gamepadNo].rgbButtons[button];
	}
	else if (button == XBOX_INPUT_LEFT)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 5 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 6 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 7;
		bool old =
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 5 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 6 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 7;

		return now && !old;
	}
	else if (button == XBOX_INPUT_RIGHT)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 1 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 2 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 3;
		bool old =
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 1 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 2 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 3;

		return now && !old;
	}
	else if (button == XBOX_INPUT_UP)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 7 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 0 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 1;
		bool old =
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 7 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 0 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 1;

		return now && !old;
	}
	else if (button == XBOX_INPUT_DOWN)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 3 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 4 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 5;
		bool old =
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 3 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 4 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 5;

		return now && !old;
	}
}

bool ControllerInput::IsPadButtonReturn(const size_t& button, const size_t& gamepadNo)
{
	if (gamepadNo >= gamepad.size())
	{
		return false;
	}
	if (button >= sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]) + 4)
	{
		return false;
	}

	if (button < sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]))
	{
		return !gamepad[gamepadNo].rgbButtons[button] && oldgamepad[gamepadNo].rgbButtons[button];
	}
	else if (button == XBOX_INPUT_LEFT)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 5 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 6 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 7;
		bool old =
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 5 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 6 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 7;

		return !now && old;
	}
	else if (button == XBOX_INPUT_RIGHT)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 1 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 2 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 3;
		bool old =
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 1 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 2 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 3;

		return !now && old;
	}
	else if (button == XBOX_INPUT_UP)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 7 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 0 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 1;
		bool old =
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 7 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 0 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 1;

		return !now && old;
	}
	else if (button == XBOX_INPUT_DOWN)
	{
		bool now =
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 3 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 4 ||
			gamepad[gamepadNo].rgdwPOV[0] == 4500 * 5;
		bool old =
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 3 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 4 ||
			oldgamepad[gamepadNo].rgdwPOV[0] == 4500 * 5;

		return !now && old;
	}
}

int ControllerInput::IsPadStick(const size_t& stickAxis, const float& deadzone, const size_t& gamepadNo)
{
	static int deadMin = 0;
	static int deadMax = 0;
	static int stick = 0;

	deadMin = zoneMin <= 0 ? (int)(zoneMin * deadzone) : (int)(zoneMin * deadzone) + zoneMin;
	deadMax = zoneMax >= 0 ? (int)(zoneMax * deadzone) : (int)(zoneMax * deadzone) + zoneMax;

	switch (stickAxis)
	{
	case INPUT_AXIS_X:
		stick = gamepad[gamepadNo].lX;
		break;
	case INPUT_AXIS_Y:
		stick = gamepad[gamepadNo].lY;
		break;
	case INPUT_AXIS_Z:
		stick = gamepad[gamepadNo].lZ;
		break;
	case INPUT_AXIS_RX:
		stick = gamepad[gamepadNo].lRx;
		break;
	case INPUT_AXIS_RY:
		stick = gamepad[gamepadNo].lRy;
		break;
	case INPUT_AXIS_RZ:
		stick = gamepad[gamepadNo].lRz;
		break;
	default:
		stick = 0;
		break;
	}

	if (stick <= deadMin && stick < 0 ||
		stick >= deadMax && stick > 0)
	{
		return stick;
	}
	else
	{
		return 0;
	}
}

HRESULT ControllerInput::CreateGamePadDevice()
{
	HRESULT hr;
	static vector<DIDEVICEINSTANCE> parameter;

	// ゲームパッドデバイスの列挙
	hr = dinput->EnumDevices(DI8DEVTYPE_GAMEPAD, &EnumGamePadCallback, &parameter, DIEDFL_ATTACHEDONLY);

	for (size_t i = 0; i < parameter.size(); i++)
	{
		devGamepad.push_back({});

		// ゲームパッドデバイスの生成
		hr = dinput->CreateDevice(parameter[i].guidInstance, &devGamepad[i], NULL);
		// 入力データ形式のセット
		hr = devGamepad[i]->SetDataFormat(&c_dfDIJoystick);
		// プロパティの設定
		hr = SetUpGamePadProperty(i);
		// 協調レベルの設定
		hr = devGamepad[i]->SetCooperativeLevel(WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	}

	return S_OK;
}

BOOL ControllerInput::EnumGamePadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	vector<DIDEVICEINSTANCE>* ptr = (vector<DIDEVICEINSTANCE>*)pvRef;
	DIDEVICEINSTANCE tmp = *lpddi;

	// 配列に格納
	ptr->push_back(tmp);

	return DIENUM_CONTINUE;
}

HRESULT ControllerInput::SetUpGamePadProperty(const size_t& gamepadNo)
{
	if (gamepadNo >= devGamepad.size())
	{
		return S_FALSE;
	}

	HRESULT hr;

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = zoneMin;
	diprg.lMax = zoneMax;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// Z軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Z;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RX軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RX;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RY軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RY;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RZ軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RZ;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}
