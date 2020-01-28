#pragma once

//---------インクルード---------
#include "main.h"
#include "sprite.h"
#include "UI.h"
#include "RGBConvert.h"

//列挙体
enum UI_PLAYER_TEXTURE	//UIに使う画像のナンバリング
{
	NUM_PLAYER_BACK,		// キャラクター背景
	NUM_PLAYER_LOUCUST,		// バッタ
	NUM_PLAYER_FIREFLY,		// 蛍
	NUM_PLAYER_BETTLE,		// カブトムシ
	NUM_PLAYER_CIRCLE,		// 周りの円

	MAX_PLAYER_UI_TEXTURE,//テクスチャの最大数
};

#define UI_PLAYER_DRAW_NUM (9)
//--------クラス-----------
class CUIPlayer:public CUI
{
public:
	// コンストラクタ
	// 引数１：プレイヤー人数
	// 引数２：プレイヤーナンバー
	// UIの配置を決める
	CUIPlayer(int num, int No);
	~CUIPlayer();
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// 作成テクスチャ
	LPDIRECT3DTEXTURE9	m_pTex[MAX_PLAYER_UI_TEXTURE];
	CSprite*			m_sprite[UI_PLAYER_DRAW_NUM];

	int m_playerType[3];

	D3DXVECTOR2 pos;
	float width;
	float height;

	HSVA hsva[3];
};
