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
			// �f�o�C�X�����X�g���Ă����疳������
			continue;
		}

		devGamepad[i]->Poll();

		if (gamepad.size() < devGamepad.size())
		{
			do
			{
				// �����킹
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
	else
	{
		return gamepad[gamepadNo].rgdwPOV[0] != 0xFFFFFFFF;
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
	else
	{
		return gamepad[gamepadNo].rgdwPOV[0] != 0xFFFFFFFF && !oldgamepad[gamepadNo].rgdwPOV[0] != 0xFFFFFFFF;
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
	else
	{
		return !gamepad[gamepadNo].rgdwPOV[0] != 0xFFFFFFFF && oldgamepad[gamepadNo].rgdwPOV[0] != 0xFFFFFFFF;
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

	if (stick >= deadMin && stick <= deadMax)
	{
		return 0;
	}
	else
	{
		return stick;
	}
}

HRESULT ControllerInput::CreateGamePadDevice()
{
	HRESULT hr;
	static vector<DIDEVICEINSTANCE> parameter;

	// �Q�[���p�b�h�f�o�C�X�̗�
	hr = dinput->EnumDevices(DI8DEVTYPE_GAMEPAD, &EnumGamePadCallback, &parameter, DIEDFL_ATTACHEDONLY);

	for (size_t i = 0; i < parameter.size(); i++)
	{
		devGamepad.push_back({});

		// �Q�[���p�b�h�f�o�C�X�̐���
		hr = dinput->CreateDevice(parameter[i].guidInstance, &devGamepad[i], NULL);
		// ���̓f�[�^�`���̃Z�b�g
		hr = devGamepad[i]->SetDataFormat(&c_dfDIJoystick);
		// �v���p�e�B�̐ݒ�
		hr = SetUpGamePadProperty(i);
		// �������x���̐ݒ�
		hr = devGamepad[i]->SetCooperativeLevel(WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	}

	return S_OK;
}

BOOL ControllerInput::EnumGamePadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	vector<DIDEVICEINSTANCE>* ptr = (vector<DIDEVICEINSTANCE>*)pvRef;
	DIDEVICEINSTANCE tmp = *lpddi;

	// �z��Ɋi�[
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

	// �����[�h���Βl���[�h�Ƃ��Đݒ�
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

	// X���̒l�͈̔͐ݒ�
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

	// Y���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// Z���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Z;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RX���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_RX;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RY���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_RY;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	// RZ���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_RZ;
	hr = devGamepad[gamepadNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}
