#pragma once

//---------インクルード---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//列挙体
enum UI_STARTCOUNT_TEXTURE	//UIに使う画像のナンバリング
{
	NUM_STARTCOUNT_1_UI_TEXTURE,
	NUM_STARTCOUNT_2_UI_TEXTURE,
	NUM_STARTCOUNT_3_UI_TEXTURE,
	NUM_STARTCOUNT_S_UI_TEXTURE,
	NUM_STARTCOUNT_T_UI_TEXTURE,
	NUM_STARTCOUNT_A_UI_TEXTURE,
	NUM_STARTCOUNT_R_UI_TEXTURE,
	NUM_STARTCOUNT_T2_UI_TEXTURE,
	MAX_STARTCOUNT_UI_TEXTURE
};
//--------クラス-----------
class CUIStartCount :public CUI
{
public:

	// コンストラクタ
	// 引数１：プレイヤー人数
	// 引数２：プレイヤーナンバー
	// UIの配置を決める
	CUIStartCount(int num, int No);

	~CUIStartCount();

	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// 作成テクスチャ
	LPDIRECT3DTEXTURE9	m_pTex[MAX_STARTCOUNT_UI_TEXTURE];
	CSprite*			m_sprite[MAX_STARTCOUNT_UI_TEXTURE];

	int m_a[MAX_STARTCOUNT_UI_TEXTURE];
	int m_time;
	int m_step;
};
