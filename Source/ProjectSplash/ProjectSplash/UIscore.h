#pragma once

//---------インクルード---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//列挙体
enum UI_SCORE_TEXTURE	//UIに使う画像のナンバリング
{
	NUM_SCORE_BORDER = 0,	// 線
	NUM_SCORE_NUM,			// 数字

	MAX_SCORE_UI_TEXTURE,	//テクスチャの最大数
};
//--------クラス-----------
class CUIScore:public CUI
{
public:

	// コンストラクタ
	// 引数１：プレイヤー人数
	// 引数２：プレイヤーナンバー
	// UIの配置を決める
	CUIScore(int num, int No);
	~CUIScore();
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// 作成テクスチャ
	LPDIRECT3DTEXTURE9	m_pTex[MAX_SCORE_UI_TEXTURE];
	CSprite*			m_sprite[MAX_SCORE_UI_TEXTURE];

	D3DXVECTOR2 pos;
	float width;
	float height;
};
