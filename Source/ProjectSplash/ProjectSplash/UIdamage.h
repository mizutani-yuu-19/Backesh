#pragma once

//---------インクルード---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//列挙体
enum UI_DAMAGE_TEXTURE	//UIに使う画像のナンバリング
{
	NUM_DAMAGE = 0,	//水残量の背景

	MAX_DAMAGE_UI_TEXTURE,//テクスチャの最大数
};
//--------クラス-----------
class CUIDamage :public CUI
{
public:

	// コンストラクタ
	// 引数１：プレイヤー人数
	// 引数２：プレイヤーナンバー
	// UIの配置を決める
	CUIDamage(int num, int No);

	~CUIDamage();

	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// 作成テクスチャ
	LPDIRECT3DTEXTURE9	m_pTex[MAX_DAMAGE_UI_TEXTURE];
	CSprite*			m_sprite[MAX_DAMAGE_UI_TEXTURE];
};
