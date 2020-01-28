// DirectInput
#include "input.h"

#pragma comment(lib, "dinput8")

//----- マクロ定義
#define MAX_JOY		8		// 最大接続数
#define JOY_MIN_X	-32768	// Ｘ軸最小値
#define JOY_MAX_X	32767	// Ｘ軸最大値
#define JOY_MIN_Y	-32768	// Ｙ軸最小値
#define JOY_MAX_Y	32767	// Ｙ軸最大値

//----- グローバル変数
static LPDIRECTINPUT8		g_pDI;							// DirectInput8 オブジェクト
static LPDIRECTINPUTDEVICE8	g_pDIDeviceKey;					// キーボード オブジェクト
static LPDIRECTINPUTDEVICE8	g_pDIDeviceJoy[MAX_JOY];		// ゲームパッド オブジェクト
static LPDIRECTINPUTDEVICE8	g_pDIDeviceMouse;				// マウス オブジェクト
static BYTE					g_bKeyState[256];				// キーボード状態
static BYTE					g_bPrevKeyState[256];			// 前回のキーボード状態
static BYTE					g_bKeyTriggerState[256];		// キーボード トリガ状態
static BYTE					g_bKeyReleaseState[256];		// キーボード リリース状態
static DIMOUSESTATE			g_diMouseState;					// マウス状態
static DIMOUSESTATE			g_diPrevMouseState;				// 前回のマウス状態
static DIMOUSESTATE			g_diMouseTriggerState;			// マウス トリガ状態
static DIMOUSESTATE			g_diMouseReleaseState;			// マウス リリース状態
static DWORD				g_dwJoystick;					// ゲームパッド接続数
static DIJOYSTATE			g_diJoyState[MAX_JOY];			// ゲームパッド状態
static DIJOYSTATE			g_diPrevJoyState[MAX_JOY];		// 前回のゲームパッド状態
static DIJOYSTATE			g_diJoyTriggerState[MAX_JOY];	// ゲームパッド トリガ状態
static DIJOYSTATE			g_diJoyReleaseState[MAX_JOY];	// ゲームパッド リリース状態
static D3DXVECTOR2			g_mousePos;

// ゲームパッド問い合わせ用コールバック関数
static BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	DIDEVCAPS diDevCaps;
	HRESULT   hr;

	hr = g_pDI->CreateDevice(lpddi->guidInstance, &g_pDIDeviceJoy[g_dwJoystick], NULL);
	if (FAILED(hr)) {
		return DIENUM_CONTINUE;
	}
	diDevCaps.dwSize = sizeof(diDevCaps);
	hr = g_pDIDeviceJoy[g_dwJoystick]->GetCapabilities(&diDevCaps);
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDIDeviceJoy[g_dwJoystick]);
		return DIENUM_CONTINUE;
	}
	if (++g_dwJoystick < MAX_JOY) return DIENUM_CONTINUE;
	return DIENUM_STOP;
}

// 初期化処理
HRESULT InitDInput(HWND hWnd)
{
	HRESULT hr;
	HINSTANCE hInst;

	hInst = (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE);

	// DirectInput オブジェクト生成
	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&g_pDI, NULL);
	if (FAILED(hr)) {
		MessageBoxA(hWnd, "DirectInputオブジェクト生成に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}
	// キーボード デバイス オブジェクト生成
	hr = g_pDI->CreateDevice(GUID_SysKeyboard, &g_pDIDeviceKey, NULL);
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDI);
		MessageBoxA(hWnd, "キーボード オブジェクト生成に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}
	// データ フォーマット設定
	hr = g_pDIDeviceKey->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDIDeviceKey);
		SAFE_RELEASE(g_pDI);
		MessageBoxA(hWnd, "キーボード データ フォーマット設定に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}
	// 協調レベルの設定
	hr = g_pDIDeviceKey->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDIDeviceKey);
		SAFE_RELEASE(g_pDI);
		MessageBoxA(hWnd, "キーボード協調レベル設定に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}
	// デバイスへの入力制御開始
	hr = g_pDIDeviceKey->Acquire();
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDIDeviceKey);
		SAFE_RELEASE(g_pDI);
		MessageBoxA(hWnd, "キーボード入力制御開始に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}

	// マウス デバイス オブジェクト生成
	hr = g_pDI->CreateDevice(GUID_SysMouse, &g_pDIDeviceMouse, NULL);
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDIDeviceKey);
		SAFE_RELEASE(g_pDI);
		MessageBoxA(hWnd, "マウス オブジェクト生成に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}
	// データ フォーマット設定
	hr = g_pDIDeviceMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDIDeviceMouse);
		SAFE_RELEASE(g_pDIDeviceKey);
		SAFE_RELEASE(g_pDI);
		MessageBoxA(hWnd, "マウス データ フォーマット設定に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}
	// 協調レベルの設定
	hr = g_pDIDeviceMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDIDeviceMouse);
		SAFE_RELEASE(g_pDIDeviceKey);
		SAFE_RELEASE(g_pDI);
		MessageBoxA(hWnd, "マウス協調レベル設定に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}
	// デバイス設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;
	g_pDIDeviceMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	// デバイスへの入力制御開始
	hr = g_pDIDeviceMouse->Acquire();
	if (FAILED(hr)) {
		SAFE_RELEASE(g_pDIDeviceMouse);
		SAFE_RELEASE(g_pDIDeviceKey);
		SAFE_RELEASE(g_pDI);
		MessageBoxA(hWnd, "マウス入力制御開始に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}
	// ゲームパッド デバイス生成
	g_dwJoystick = 0;
	hr = g_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr)) {
		for (DWORD i = 0; i < g_dwJoystick; ++i)
			SAFE_RELEASE(g_pDIDeviceJoy[i]);
		SAFE_RELEASE(g_pDIDeviceMouse);
		SAFE_RELEASE(g_pDIDeviceKey);
		SAFE_RELEASE(g_pDI);
		MessageBoxA(hWnd, "ゲームパッド オブジェクト生成に失敗しました。", "error", MB_ICONSTOP | MB_OK);
		return hr;
	}
	for (DWORD i = 0; i < g_dwJoystick; ++i) {
		// データ フォーマット設定
		hr = g_pDIDeviceJoy[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr)) {
			for (DWORD j = 0; j < g_dwJoystick; ++j)
				SAFE_RELEASE(g_pDIDeviceJoy[j]);
			SAFE_RELEASE(g_pDIDeviceMouse);
			SAFE_RELEASE(g_pDIDeviceKey);
			SAFE_RELEASE(g_pDI);
			MessageBoxA(hWnd, "ゲームパッド データ フォーマット設定に失敗しました。", "error", MB_ICONSTOP | MB_OK);
			return hr;
		}
		// 協調レベルの設定
		hr = g_pDIDeviceJoy[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr)) {
			for (DWORD j = 0; j < g_dwJoystick; ++j)
				SAFE_RELEASE(g_pDIDeviceJoy[j]);
			SAFE_RELEASE(g_pDIDeviceMouse);
			SAFE_RELEASE(g_pDIDeviceKey);
			SAFE_RELEASE(g_pDI);
			MessageBoxA(hWnd, "ゲームパッド協調レベル設定に失敗しました。", "error", MB_ICONSTOP | MB_OK);
			return hr;
		}
		// デバイス設定
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = JOY_MIN_X;
		diprg.lMax = JOY_MAX_X;
		g_pDIDeviceJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = JOY_MIN_Y;
		diprg.lMax = JOY_MAX_Y;
		g_pDIDeviceJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// デバイスへの入力制御開始
		hr = g_pDIDeviceJoy[i]->Acquire();
		if (FAILED(hr)) {
			for (DWORD j = 0; j < g_dwJoystick; ++j)
				SAFE_RELEASE(g_pDIDeviceJoy[j]);
			SAFE_RELEASE(g_pDIDeviceMouse);
			SAFE_RELEASE(g_pDIDeviceKey);
			SAFE_RELEASE(g_pDI);
			MessageBoxA(hWnd, "マウス入力制御開始に失敗しました。", "error", MB_ICONSTOP | MB_OK);
			return hr;
		}
	}

	return hr;
}

// 終了処理
void UninitDInput()
{
	for (DWORD i = 0; i < g_dwJoystick; ++i) {
		if (g_pDIDeviceJoy[i]) {
			g_pDIDeviceJoy[i]->Unacquire();
		}
		SAFE_RELEASE(g_pDIDeviceJoy[i]);
	}
	if (g_pDIDeviceMouse) {
		g_pDIDeviceMouse->Unacquire();
	}
	SAFE_RELEASE(g_pDIDeviceKey);
	if (g_pDIDeviceKey) {
		g_pDIDeviceKey->Unacquire();
	}
	SAFE_RELEASE(g_pDIDeviceKey);
}

// 更新処理
void UpdateDInput()
{
	HRESULT hr;

	if (g_pDIDeviceKey) {
		g_pDIDeviceKey->Acquire();
		CopyMemory(g_bPrevKeyState, g_bKeyState, 256);
		hr = g_pDIDeviceKey->GetDeviceState(256, g_bKeyState);
		if (hr == DIERR_INPUTLOST)
			g_pDIDeviceKey->Acquire();
		else {
			for (int i = 0; i < 256; ++i) {
				g_bKeyTriggerState[i] = (~g_bPrevKeyState[i] & g_bKeyState[i]) & 0x80;
				g_bKeyReleaseState[i] = (g_bPrevKeyState[i] & ~g_bKeyState[i]) & 0x80;
			}
		}
	}
	if (g_pDIDeviceMouse) {
		g_pDIDeviceMouse->Acquire();
		CopyMemory(&g_diPrevMouseState, &g_diMouseState, sizeof(g_diMouseState));
		hr = g_pDIDeviceMouse->GetDeviceState(sizeof(g_diMouseState), &g_diMouseState);
		if (hr == DIERR_INPUTLOST)
			g_pDIDeviceMouse->Acquire();
		else {
			for (int i = 0; i < 4; ++i) {
				g_diMouseTriggerState.rgbButtons[i] = (~g_diPrevMouseState.rgbButtons[i] & g_diMouseState.rgbButtons[i]) & 0x80;
				g_diMouseReleaseState.rgbButtons[i] = (g_diPrevMouseState.rgbButtons[i] & ~g_diMouseState.rgbButtons[i]) & 0x80;
			}
		}
	}
	for (DWORD i = 0; i < g_dwJoystick; ++i) {
		g_pDIDeviceJoy[i]->Acquire();
		g_pDIDeviceJoy[i]->Poll();
		CopyMemory(&g_diPrevJoyState[i], &g_diJoyState[i], sizeof(g_diJoyState[0]));
		hr = g_pDIDeviceJoy[i]->GetDeviceState(sizeof(g_diJoyState[0]), &g_diJoyState[i]);
		if (hr == DIERR_INPUTLOST)
			g_pDIDeviceJoy[i]->Acquire();
		else {
			for (int j = 0; j < 32; ++j) {
				g_diJoyTriggerState[i].rgbButtons[j] = (~g_diPrevJoyState[i].rgbButtons[j] & g_diJoyState[i].rgbButtons[j]) & 0x80;
				g_diJoyReleaseState[i].rgbButtons[j] = (g_diPrevJoyState[i].rgbButtons[j] & ~g_diJoyState[i].rgbButtons[j]) & 0x80;
			}
		}
	}
}

// キー押下状態取得
bool DIGetKey(DWORD dwKey)
{
	if (dwKey >= _countof(g_bKeyState)) return false;
	return (g_bKeyState[dwKey] & 0x80) != 0;
}

// キー トリガ状態取得
bool DIGetKeyTrigger(DWORD dwKey)
{
	if (dwKey >= _countof(g_bKeyTriggerState)) return false;
	return (g_bKeyTriggerState[dwKey] & 0x80) != 0;
}

// キー リリース状態取得
bool DIGetKeyRelease(DWORD dwKey)
{
	if (dwKey >= _countof(g_bKeyReleaseState)) return false;
	return (g_bKeyReleaseState[dwKey] & 0x80) != 0;
}

// ゲームパッド数取得
DWORD DIGetJoyCount()
{
	return g_dwJoystick;
}

// ゲームパッド状態取得
DIJOYSTATE *DIGetJoy(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return NULL;
	return &g_diJoyState[dwJoy];
}

// ゲームパッドLX軸取得
LONG DIGetJoyLX(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lX;
}

// ゲームパッドLY軸取得
LONG DIGetJoyLY(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lY;
}

// ゲームパッドLZ軸取得
LONG DIGetJoyLZ(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lZ;
}

// ゲームパッドRX軸取得
LONG DIGetJoyRX(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lRx;
}

// ゲームパッドRY軸取得
LONG DIGetJoyRY(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lRy;
}

// ゲームパッドRZ軸取得
LONG DIGetJoyRZ(DWORD dwJoy)
{
	if (dwJoy >= g_dwJoystick) return 0;
	return g_diJoyState[dwJoy].lRz;
}

// ゲームパッド ボタン押下状態取得
bool DIGetJoyButton(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= g_dwJoystick) return false;
	if (dwBtn >= _countof(g_diJoyState[dwJoy].rgbButtons)) return false;
	return (g_diJoyState[dwJoy].rgbButtons[dwBtn] & 0x80) != 0;
}

// ゲームパッド ボタン トリガ状態取得
bool DIGetJoyTrigger(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= g_dwJoystick) return false;
	if (dwBtn >= _countof(g_diJoyTriggerState[dwJoy].rgbButtons)) return false;
	return (g_diJoyTriggerState[dwJoy].rgbButtons[dwBtn] & 0x80) != 0;
}

// ゲームパッド ボタン リリース状態取得
bool DIGetJoyRelease(DWORD dwJoy, DWORD dwBtn)
{
	if (dwJoy >= g_dwJoystick) return false;
	if (dwBtn >= _countof(g_diJoyReleaseState[dwJoy].rgbButtons)) return false;
	return (g_diJoyReleaseState[dwJoy].rgbButtons[dwBtn] & 0x80) != 0;
}

// マウス状態取得
DIMOUSESTATE *DIGetMouse()
{
	return &g_diMouseState;
}

// マウス ボタン押下状態取得
bool DIGetMouseButton(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_diMouseState.rgbButtons)) return false;
	return (g_diMouseState.rgbButtons[dwBtn] & 0x80) != 0;
}

// マウス ボタン トリガ状態取得
bool DIGetMouseTrigger(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_diMouseTriggerState.rgbButtons)) return false;
	return (g_diMouseTriggerState.rgbButtons[dwBtn] & 0x80) != 0;
}

// マウス ボタン リリース状態取得
bool DIGetMouseRelease(DWORD dwBtn)
{
	if (dwBtn >= _countof(g_diMouseReleaseState.rgbButtons)) return false;
	return (g_diMouseReleaseState.rgbButtons[dwBtn] & 0x80) != 0;
}


void SetMousePos(int x, int y) {
	g_mousePos.x = x;
	g_mousePos.y = y;
}

D3DXVECTOR2 GetMousePos() {
	return g_mousePos;
}