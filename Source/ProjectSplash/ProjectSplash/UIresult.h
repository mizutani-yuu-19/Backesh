#pragma once

//---------インクルード---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//列挙体
enum UI_RESULT_TEXTURE	//UIに使う画像のナンバリング
{
	NUM_WINNER = 0,	//水残量の背景

	MAX_RESULT_UI_TEXTURE,//テクスチャの最大数
};
//--------クラス-----------
class CUIResult :public CUI
{
public:

	// コンストラクタ
	// 引数１：プレイヤー人数
	// 引数２：プレイヤーナンバー
	// UIの配置を決める
	CUIResult(int num, int No);

	~CUIResult();

	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// 作成テクスチャ
	LPDIRECT3DTEXTURE9	m_pTex[MAX_RESULT_UI_TEXTURE];
	CSprite*			m_sprite[MAX_RESULT_UI_TEXTURE];
};
