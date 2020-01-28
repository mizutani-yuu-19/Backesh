//=============================================================================
//
// XBOXコントローラの情報取得
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

#define PAD_A					VK_PAD_A				//A ボタン
#define PAD_B					VK_PAD_B				//B ボタン
#define PAD_X					VK_PAD_X				//X ボタン
#define PAD_Y					VK_PAD_Y				//Y ボタン
#define PAD_RIGHT_BUTTON		VK_PAD_RSHOULDER		//右ショルダ ボタン
#define PAD_LEFT_BUTTON			VK_PAD_LSHOULDER		//左ショルダ ボタン
#define PAD_LEFT_TRIGGER		VK_PAD_LTRIGGER			//左トリガー
#define PAD_RIGHT_TRIGGER		VK_PAD_RTRIGGER			//右トリガー
#define PAD_UP_CROSS			VK_PAD_DPAD_UP			//方向パッドの上方向
#define PAD_DOWN_CROSS			VK_PAD_DPAD_DOWN		//方向パッドの下方向
#define PAD_LEFT_CROSS			VK_PAD_DPAD_LEFT		//方向パッドの左方向
#define PAD_RIGHT_CROSS			VK_PAD_DPAD_RIGHT		//方向パッドの右方向
#define PAD_START				VK_PAD_START			//START ボタン
#define PAD_BACK				VK_PAD_BACK				//BACK ボタン
#define PAD_LSTICK_PRESS		VK_PAD_LTHUMB_PRESS		//左サムスティックのクリック
#define PAD_RSTICK_PRESS		VK_PAD_RTHUMB_PRESS		//右サムスティックのクリック
#define PAD_LSTICK_UP			VK_PAD_LTHUMB_UP		//左サムスティックの上方向
#define PAD_LSTICK_DOWN			VK_PAD_LTHUMB_DOWN		//左サムスティックの下方向
#define PAD_LSTICK_RIGHT		VK_PAD_LTHUMB_RIGHT		//左サムスティックの右方向
#define PAD_LSTICK_LEFT			VK_PAD_LTHUMB_LEFT		//左サムスティックの左方向
#define PAD_LSTICK_UPLEFT		VK_PAD_LTHUMB_UPLEFT	//左サムスティックの上方向および左方向
#define PAD_LSTICK_UPRIGH		VK_PAD_LTHUMB_UPRIGHT	//左サムスティックの上方向および右方向
#define PAD_LSTICK_DOWNRIGHT	VK_PAD_LTHUMB_DOWNRIGHT	//左サムスティックの下方向および右方向
#define PAD_LSTICK_DOWNLEFT		VK_PAD_LTHUMB_DOWNLEFT	//左サムスティックの下方向および左方向
#define PAD_RSTICK_UP			VK_PAD_RTHUMB_UP		//右サムスティックの上方向
#define PAD_RSTICK_DOWN			VK_PAD_RTHUMB_DOWN		//右サムスティックの下方向
#define PAD_RSTICK_RIGHT		VK_PAD_RTHUMB_RIGHT		//右サムスティックの右方向
#define PAD_RSTICK_LEFT			VK_PAD_RTHUMB_LEFT		//右サムスティックの左方向
#define PAD_RSTICK_UPLEFT		VK_PAD_RTHUMB_UPLEFT	//右サムスティックの上方向および左方向
#define PAD_RSTICK_UPRIGHT		VK_PAD_RTHUMB_UPRIGHT	//右サムスティックの上方向および右方向
#define PAD_RSTICK_DOWNRIGHT	VK_PAD_RTHUMB_DOWNRIGHT	//右サムスティックの下方向および右方向
#define PAD_RSTICK_DOWNLEFT		VK_PAD_RTHUMB_DOWNLEFT	//右サムスティックの下方向および左方向

#define PAD_BUTTON_MAX			(32)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void UpdatePad();
// 引数１：何個目のコントローラ？
// 引数２：フラグ　　　　例PAD_FLAG_PRESS
// 引数３：どのボタン？　例PAD_UP_CROSS
bool GetPadButton(int no, int flag, int button);

// 引数１：何個目のコントローラ？
float GetPadLEFTStickX(int no);

// 引数１：何個目のコントローラ？
float GetPadLEFTStickY(int no);

// 引数１：何個目のコントローラ？
float GetPadRIGHTStickX(int no);

// 引数１：何個目のコントローラ？
float GetPadRIGHTStickY(int no);

// 引数１：何個目のコントローラ？
float GetPadLEFTTrigger(int no);

// 引数１：何個目のコントローラ？
float GetPadRIGHTTrigger(int no);

int GetPadNum();

// 引数１：何個目のコントローラ？
// 引数２：フラグ　　　　例PAD_FLAG_PRESS
// 引数３：方向			 例PAD_STICK_UP
//bool  GetPadLEFTStick(int no, int flag, int direct);

#endif
