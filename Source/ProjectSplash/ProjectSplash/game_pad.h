//=============================================================================
//
// XBOX�R���g���[���̏��擾
//
//=============================================================================
#ifndef _GAME_PAD_H_
#define _GAME_PAD_H_

#include "main.h"

#define MAX_PAD		(4)
#define MAX_BUTTON	(14)

#define	PAD_FLAG_TRIGGER	(0)
#define	PAD_FLAG_RELEASE	(1)
#define	PAD_FLAG_PRESS		(2)
#define PAD_FLAG_MAX		(3)

#define PAD_A					VK_PAD_A				//A �{�^��
#define PAD_B					VK_PAD_B				//B �{�^��
#define PAD_X					VK_PAD_X				//X �{�^��
#define PAD_Y					VK_PAD_Y				//Y �{�^��
#define PAD_RIGHT_BUTTON		VK_PAD_RSHOULDER		//�E�V�����_ �{�^��
#define PAD_LEFT_BUTTON			VK_PAD_LSHOULDER		//���V�����_ �{�^��
#define PAD_LEFT_TRIGGER		VK_PAD_LTRIGGER			//���g���K�[
#define PAD_RIGHT_TRIGGER		VK_PAD_RTRIGGER			//�E�g���K�[
#define PAD_UP_CROSS			VK_PAD_DPAD_UP			//�����p�b�h�̏����
#define PAD_DOWN_CROSS			VK_PAD_DPAD_DOWN		//�����p�b�h�̉�����
#define PAD_LEFT_CROSS			VK_PAD_DPAD_LEFT		//�����p�b�h�̍�����
#define PAD_RIGHT_CROSS			VK_PAD_DPAD_RIGHT		//�����p�b�h�̉E����
#define PAD_START				VK_PAD_START			//START �{�^��
#define PAD_BACK				VK_PAD_BACK				//BACK �{�^��
#define PAD_LSTICK_PRESS		VK_PAD_LTHUMB_PRESS		//���T���X�e�B�b�N�̃N���b�N
#define PAD_RSTICK_PRESS		VK_PAD_RTHUMB_PRESS		//�E�T���X�e�B�b�N�̃N���b�N
#define PAD_LSTICK_UP			VK_PAD_LTHUMB_UP		//���T���X�e�B�b�N�̏����
#define PAD_LSTICK_DOWN			VK_PAD_LTHUMB_DOWN		//���T���X�e�B�b�N�̉�����
#define PAD_LSTICK_RIGHT		VK_PAD_LTHUMB_RIGHT		//���T���X�e�B�b�N�̉E����
#define PAD_LSTICK_LEFT			VK_PAD_LTHUMB_LEFT		//���T���X�e�B�b�N�̍�����
#define PAD_LSTICK_UPLEFT		VK_PAD_LTHUMB_UPLEFT	//���T���X�e�B�b�N�̏��������э�����
#define PAD_LSTICK_UPRIGH		VK_PAD_LTHUMB_UPRIGHT	//���T���X�e�B�b�N�̏��������щE����
#define PAD_LSTICK_DOWNRIGHT	VK_PAD_LTHUMB_DOWNRIGHT	//���T���X�e�B�b�N�̉���������щE����
#define PAD_LSTICK_DOWNLEFT		VK_PAD_LTHUMB_DOWNLEFT	//���T���X�e�B�b�N�̉���������э�����
#define PAD_RSTICK_UP			VK_PAD_RTHUMB_UP		//�E�T���X�e�B�b�N�̏����
#define PAD_RSTICK_DOWN			VK_PAD_RTHUMB_DOWN		//�E�T���X�e�B�b�N�̉�����
#define PAD_RSTICK_RIGHT		VK_PAD_RTHUMB_RIGHT		//�E�T���X�e�B�b�N�̉E����
#define PAD_RSTICK_LEFT			VK_PAD_RTHUMB_LEFT		//�E�T���X�e�B�b�N�̍�����
#define PAD_RSTICK_UPLEFT		VK_PAD_RTHUMB_UPLEFT	//�E�T���X�e�B�b�N�̏��������э�����
#define PAD_RSTICK_UPRIGHT		VK_PAD_RTHUMB_UPRIGHT	//�E�T���X�e�B�b�N�̏��������щE����
#define PAD_RSTICK_DOWNRIGHT	VK_PAD_RTHUMB_DOWNRIGHT	//�E�T���X�e�B�b�N�̉���������щE����
#define PAD_RSTICK_DOWNLEFT		VK_PAD_RTHUMB_DOWNLEFT	//�E�T���X�e�B�b�N�̉���������э�����

#define PAD_BUTTON_MAX			(32)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void UpdatePad();
// �����P�F���ڂ̃R���g���[���H
// �����Q�F�t���O�@�@�@�@��PAD_FLAG_PRESS
// �����R�F�ǂ̃{�^���H�@��PAD_UP_CROSS
bool GetPadButton(int no, int flag, int button);

// �����P�F���ڂ̃R���g���[���H
float GetPadLEFTStickX(int no);

// �����P�F���ڂ̃R���g���[���H
float GetPadLEFTStickY(int no);

// �����P�F���ڂ̃R���g���[���H
float GetPadRIGHTStickX(int no);

// �����P�F���ڂ̃R���g���[���H
float GetPadRIGHTStickY(int no);

// �����P�F���ڂ̃R���g���[���H
float GetPadLEFTTrigger(int no);

// �����P�F���ڂ̃R���g���[���H
float GetPadRIGHTTrigger(int no);

int GetPadNum();

// �����P�F���ڂ̃R���g���[���H
// �����Q�F�t���O�@�@�@�@��PAD_FLAG_PRESS
// �����R�F����			 ��PAD_STICK_UP
//bool  GetPadLEFTStick(int no, int flag, int direct);

#endif
