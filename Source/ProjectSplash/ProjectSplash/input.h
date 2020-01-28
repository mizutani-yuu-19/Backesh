// DirectInput
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION	0x0800	// DirectX8 を明示
#include <dinput.h>					// DirectInput 関連のヘッダ

#include "main.h"

// プロトタイプ宣言
HRESULT InitDInput(HWND hWnd);
void UninitDInput();
void UpdateDInput();

bool DIGetKey(DWORD dwKey);
bool DIGetKeyTrigger(DWORD dwKey);
bool DIGetKeyRelease(DWORD dwKey);
DWORD DIGetJoyCount();
DIJOYSTATE *DIGetJoy(DWORD dwJoy);
LONG DIGetJoyLX(DWORD dwJoy);
LONG DIGetJoyLY(DWORD dwJoy);
LONG DIGetJoyLZ(DWORD dwJoy);
LONG DIGetJoyRX(DWORD dwJoy);
LONG DIGetJoyRY(DWORD dwJoy);
LONG DIGetJoyRZ(DWORD dwJoy);
bool DIGetJoyButton(DWORD dwJoy, DWORD dwBtn);
bool DIGetJoyTrigger(DWORD dwJoy, DWORD dwBtn);
bool DIGetJoyRelease(DWORD dwJoy, DWORD dwBtn);
DIMOUSESTATE *DIGetMouse();
bool DIGetMouseButton(DWORD dwBtn);
bool DIGetMouseTrigger(DWORD dwBtn);
bool DIGetMouseRelease(DWORD dwBtn);
void SetMousePos(int x, int y);
D3DXVECTOR2 GetMousePos();
#endif