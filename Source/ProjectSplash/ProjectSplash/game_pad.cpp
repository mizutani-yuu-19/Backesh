//=============================================================================
//
// XBOXコントローラの情報取得
//
//=============================================================================
#include "game_pad.h"
#include "debugproc.h"

typedef struct {
	int padNo;
	int button;
	int flag;
}STROKE;

#define INPUT_DEADZONE  ( 0.20f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
std::vector<STROKE*>	g_stroke;
static XINPUT_STATE g_pad[MAX_PAD];
XINPUT_KEYSTROKE g_strokePad;

//----------プロトタイプ-----------
void UpdatePad() {
	for (int i = 0; i < g_stroke.size(); i++) {
		if (g_stroke[i]->flag == PAD_FLAG_RELEASE) {
			delete g_stroke[i];
			g_stroke.erase(g_stroke.begin() + i);
			i--;
			continue;
		}
		if (g_stroke[i]->flag == PAD_FLAG_TRIGGER) {
			g_stroke[i]->flag = PAD_FLAG_PRESS;
		}
	}
	for (int i = 0; i < MAX_PAD; i++) {
		if (!XInputGetState(i, &g_pad[i])) {	// キー状態取得
#if _DEBUG
			float lx, ly, rx, ry, rt, lt;
			{
				if ((g_pad[i].Gamepad.sThumbRX < INPUT_DEADZONE &&
					g_pad[i].Gamepad.sThumbRX > -INPUT_DEADZONE) &&
					(g_pad[i].Gamepad.sThumbRY < INPUT_DEADZONE &&
						g_pad[i].Gamepad.sThumbRY > -INPUT_DEADZONE))
				{
					g_pad[i].Gamepad.sThumbRX = 0;
					g_pad[i].Gamepad.sThumbRY = 0;
				}
				lx = g_pad[i].Gamepad.sThumbLX / 32767.0f;
				ly = g_pad[i].Gamepad.sThumbLY / 32767.0f;
				rx = g_pad[i].Gamepad.sThumbRX / 32767.0f;
				ry = g_pad[i].Gamepad.sThumbRY / 32767.0f;
				rt = g_pad[i].Gamepad.bRightTrigger / 255.0f;
				lt = g_pad[i].Gamepad.bLeftTrigger / 255.0f;
				PrintDebugProc("\n");
				PrintDebugProc("LX:%f", lx);
				PrintDebugProc("LY:%f", ly);
				PrintDebugProc("LT:%f", lt);
				PrintDebugProc("\n");
				PrintDebugProc("RX:%f", rx);
				PrintDebugProc("RY:%f", ry);
				PrintDebugProc("RT:%f", rt);
			}
#endif
		}
		if (ERROR_SUCCESS == XInputGetKeystroke(i, 0, &g_strokePad)) {
			bool triggerFlag = true;
			// すでに押されているか
			for (int j = 0; j < g_stroke.size(); j++) {
				if (g_stroke[j]->padNo != g_strokePad.UserIndex || g_strokePad.VirtualKey != g_stroke[j]->button)	continue;
				switch (g_strokePad.Flags)
				{
				case XINPUT_KEYSTROKE_KEYUP:
					g_stroke[j]->flag = PAD_FLAG_RELEASE;
					break;
				case 5:
					g_stroke[j]->flag = PAD_FLAG_PRESS;
					break;
				default:
					break;
				}
				triggerFlag = false;
			}
			// まだ押されてない
			if (triggerFlag&&g_strokePad.Flags != XINPUT_KEYSTROKE_KEYUP) {
				STROKE *temp = new STROKE;
				temp->button = g_strokePad.VirtualKey;
				temp->flag = PAD_FLAG_TRIGGER;
				temp->padNo = g_strokePad.UserIndex;
				g_stroke.push_back(temp);
			}
		}
	}
}

bool GetPadButton(int no, int pat, int button) {
	for (int i = 0; i < g_stroke.size(); i++) {
		if (g_stroke[i]->flag == pat && g_stroke[i]->button == button && g_stroke[i]->padNo == no) {
			return true;
		}
	}
	return false;
}
float GetPadLEFTStickX(int no) {
	if (g_pad[no].Gamepad.sThumbLX < INPUT_DEADZONE &&
		g_pad[no].Gamepad.sThumbLX > -INPUT_DEADZONE)
	{
		return 0.0f;
	}
	else
		return g_pad[no].Gamepad.sThumbLX / 32767.0f;
}
float GetPadLEFTStickY(int no) {
	if (g_pad[no].Gamepad.sThumbLY < INPUT_DEADZONE &&
		g_pad[no].Gamepad.sThumbLY > -INPUT_DEADZONE)
	{
		return 0;
	}
	else
		return g_pad[no].Gamepad.sThumbLY / 32767.0f;
}
float GetPadRIGHTStickX(int no) {
	if (g_pad[no].Gamepad.sThumbRX < INPUT_DEADZONE &&
		g_pad[no].Gamepad.sThumbRX > -INPUT_DEADZONE)
	{
		return 0;
	}
	else
		return g_pad[no].Gamepad.sThumbRX / 32767.0f;
}
float GetPadRIGHTStickY(int no) {
	if (g_pad[no].Gamepad.sThumbRY < INPUT_DEADZONE &&
		g_pad[no].Gamepad.sThumbRY > -INPUT_DEADZONE)
	{
		return 0;
	}
	else
		return g_pad[no].Gamepad.sThumbRY / 32767.0f;
}
float GetPadLEFTTrigger(int no) {
	return g_pad[no].Gamepad.bLeftTrigger / 255.0f;
}
float GetPadRIGHTTrigger(int no) {
	return g_pad[no].Gamepad.bRightTrigger / 255.0f;
}

// コントローラ数取得
int GetPadNum() {
	int num=0;
	for (int i = 0; i < MAX_PAD; i++) {
		if (!XInputGetState(i, &g_pad[i]))	num++;
	}
	return num;
}