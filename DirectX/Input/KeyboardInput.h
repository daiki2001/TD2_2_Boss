#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"

class KeyboardInput final
{
public:
	static void Initialize();
	static void Update();
	
	static bool GetKeyPressT(int key);
	static bool GetKeyReleasT(int key);
	static int GetKeyPress(int key);
	static int GetKeyReleas(int key);

private:
	static HRESULT result;
	static int pressCount[256];
	static int releasCount[256];
};

