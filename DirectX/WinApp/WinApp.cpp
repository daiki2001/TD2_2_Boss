#include "WinApp.h"


const wchar_t WinApp::windowClassName[] = L"Re:Strike";
WNDCLASSEX WinApp::wndClass;
HWND WinApp::hwnd = nullptr;


LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージで分岐
	switch (msg) {
	case WM_DESTROY: // ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::WindowCreate()
{
		//ウィンドウクラスの設定
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc;	//ウィンドウプロ―ジャを設定
	wndClass.lpszClassName = windowClassName;	//ウィンドウクラス名
	wndClass.hInstance = GetModuleHandle(nullptr);	//ウィンドウハンドル
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&wndClass);

	//ウィンドウサイズ{x,y,横幅,縦幅}
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(
		wndClass.lpszClassName,
		windowClassName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wndClass.hInstance,
		nullptr
		);

	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::WindowClassRelease()
{
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

bool WinApp::WindowsMessage()
{
	MSG msg{};
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // プロシージャにメッセージを送る
	}

	// ✖ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return false;
	}

	return true;
}
