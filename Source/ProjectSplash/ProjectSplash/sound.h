//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
#include <XAudio2.h>

#define USE_SOUND
//*****************************************************************************
// �T�E���h�t�@�C��
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
	SOUND_LABEL_MAX,			// �o�^���̎����ݒ�
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

// �������͉���0.0f�`1.0f�̊�
HRESULT PlaySound(int label,float vol);

void StopSound(int label);
void StopSound(void);

#endif