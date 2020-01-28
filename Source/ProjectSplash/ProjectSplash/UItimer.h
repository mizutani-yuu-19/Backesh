#pragma once

//---------インクルード---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//列挙体
enum UI_TIMER_TEXTURE	//UIに使う画像のナンバリング
{
	NUM_COLON = 0,		// コロン
	NUM_TIMER,			// 数字

	MAX_TIMER_UI_TEXTURE,//テクスチャの最大数
};
//--------クラス-----------
class CUITimer :public CUI
{
public:
	CUITimer(int num, int No);
	~CUITimer();

	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// 作成テクスチャ
	LPDIRECT3DTEXTURE9	m_pTex[MAX_TIMER_UI_TEXTURE];
	CSprite*			m_sprite[MAX_TIMER_UI_TEXTURE];

	D3DXVECTOR2 pos;
	float width;
	float height;
};
