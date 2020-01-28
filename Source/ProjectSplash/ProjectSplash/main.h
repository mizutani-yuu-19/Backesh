//=============================================================================
//
// メイン処理 [main.h]
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#include <XInput.h>
#include <vector>
#include <list>
#include <string>

#define DIRECTINPUT_VERSION (0x0800)	// 警告対策
#include "dinput.h"

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "Xinput.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#ifdef _DEBUG
//#define SCREEN_WIDTH	(683)				// ウインドウの幅
//#define SCREEN_HEIGHT	(384)				// ウインドウの高さ
#define SCREEN_WIDTH	(1366)				// ウインドウの幅
#define SCREEN_HEIGHT	(768)				// ウインドウの高さ
#else
#define SCREEN_WIDTH	(1366)				// ウインドウの幅
#define SCREEN_HEIGHT	(768)				// ウインドウの高さ
//#define SCREEN_WIDTH	(1920)				// ウインドウの幅
//#define SCREEN_HEIGHT	(1080)				// ウインドウの高さ
#endif // _DEBUG

#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)		// 頂点数
#define	NUM_POLYGON		(2)		// ポリゴン数

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
{if(x){x->Release();x=NULL;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) \
{if(x){delete x;x=NULL;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) \
{if(x){delete[] x;x=NULL;}}
#endif
#ifndef unite
#define unite(a,b) a ## b
#endif
// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtxPos;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 uv;		// テクスチャ座標
} VERTEX_2D;

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
#endif