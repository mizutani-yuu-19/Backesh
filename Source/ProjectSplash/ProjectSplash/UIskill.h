#pragma once

//---------インクルード---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//列挙体
enum UI_SKILL_TEXTURE	//UIに使う画像のナンバリング
{
	NUM_SKILL_BOTTLE = 0,	//水残量の背景
	NUM_SKILL_COLOR,		//水量
	NUM_SKILL_STAR,		//必殺技

	MAX_SKILL_UI_TEXTURE,//テクスチャの最大数
};
//--------クラス-----------
class CUISkill:public CUI
{
public:

	// コンストラクタ
	// 引数１：プレイヤー人数
	// 引数２：プレイヤーナンバー
	// UIの配置を決める
	CUISkill(int num, int No);
	~CUISkill();
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// 作成テクスチャ
	LPDIRECT3DTEXTURE9	m_pTex[MAX_SKILL_UI_TEXTURE];
	CSprite*			m_sprite[MAX_SKILL_UI_TEXTURE];

	D3DXVECTOR2 pos;
	float width;
	float height;
};
