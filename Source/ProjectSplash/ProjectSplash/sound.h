//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
#include <XAudio2.h>

#define USE_SOUND
//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE = 0,
	SOUND_LABEL_SELECT,
	SOUND_LABEL_BATTLE,
	SOUND_LABEL_GAMEOVER,
	SOUND_LABEL_WIN,
	SOUND_LABEL_ASIOTO1,
	SOUND_LABEL_ASIOTO2,
	SOUND_LABEL_ASIOTO3,
	SOUND_LABEL_ASIOTO4,
	SOUND_LABEL_ASIOTO5,
	SOUND_LABEL_ASIOTO6,
	SOUND_LABEL_ASIOTO7,
	SOUND_LABEL_ASIOTO8,
	SOUND_LABEL_ASIOTO9,
	SOUND_LABEL_ASIOTO10,
	SOUND_LABEL_ASIOTO11,
	SOUND_LABEL_ASIOTO12,
	SOUND_LABEL_SPLASH1,
	SOUND_LABEL_SPLASH2,
	SOUND_LABEL_SPLASH3,
	SOUND_LABEL_SPLASH4,
	SOUND_LABEL_SPLASH5,
	SOUND_LABEL_SPLASH6,
	SOUND_LABEL_SPLASH7,
	SOUND_LABEL_SPLASH8,
	SOUND_LABEL_SPLASH9,
	SOUND_LABEL_SPLASH10,
	SOUND_LABEL_SPLASH11,
	SOUND_LABEL_SPLASH12,
	SOUND_LABEL_MAX,			// 登録数の自動設定
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

// 第二引数は音量0.0f～1.0fの間
HRESULT PlaySound(int label,float vol);

void StopSound(int label);
void StopSound(void);

#endif