//----------インクルード---------
#include "sprite.h"
#include <math.h>
//----------定数マクロ----------

//----------メソッド--------------
// コンストラクタ
CSprite::CSprite() {
	m_pos = D3DXVECTOR2(0.0f, 0.0f);				// 位置
	m_color = D3DCOLOR_RGBA(255, 255, 255, 255);	// 色
	m_angleDegree = 0.0f;							// 回転角度（°）
	m_scale.x = 1.0f;								// 拡大率
	m_scale.y = 1.0f;								// 拡大率
	m_width = 1.0f;									// 幅
	m_height = 1.0f;								// 高さ
	m_divideX = 1;									// 分割数横
	m_divideY = 1;									// 分割数縦
	m_patNum = 1;
	m_direction = 1;								// 正の向き

	for (int i = 0; i < 4; ++i)
	{
		m_vertex[i].rhw = 1.0f;
		m_vertex[i].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);
		m_vertex[i].uv.x = i % 2;
		m_vertex[i].uv.y = i * 0.5f;
		m_vertex[i].vtxPos.z = 0.0f;
	}
}
// デストラクタ
CSprite::~CSprite() {

}

void CSprite::InitSprite(D3DXVECTOR2 pos, float width, float height, int divideX, int divideY) {
	m_pos = pos;
	m_width = width;
	m_height = height;
	m_divideX = divideX;
	m_divideY = divideY;
	m_scale.x = 1.0f;								// 拡大率
	m_scale.y = 1.0f;								// 拡大率

	for (int i = 0; i < 4; ++i) {
		m_vertex[i].rhw = 1.0f;
		m_vertex[i].diffuse = D3DCOLOR_ARGB(255, 255, 255, 255);
	}
}

void CSprite::DrawSprite(LPDIRECT3DTEXTURE9 pTexture) {
	float rad = D3DXToRadian(m_angleDegree);  //D3DXToRadian角度をradianに変換

	float s = sinf(rad);
	float c = cosf(rad);

	for (int i = 0; i < 4; ++i) {
		// アニメーション
		int u = m_patNum % m_divideX;
		int v = m_patNum / m_divideX;
		m_vertex[i].uv.x = (u + i % 2) * (1.0f / m_divideX) * m_direction;
		m_vertex[i].uv.y = (int)(v + i * 0.5f) * (1.0f / m_divideY);
		// 位置
		float x = ((i & 1)  * m_width - m_width * 0.5f) * m_scale.x;
		float y = ((i >> 1) * m_height - m_height * 0.5f) * m_scale.y;
		m_vertex[i].vtxPos.x = m_pos.x + x * c - y * s;
		m_vertex[i].vtxPos.y = m_pos.y + x * s + y * c;
		// カラー
		m_vertex[i].diffuse = m_color;
	}

	// デバイス取得
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	// 頂点書式設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャ設定
	pDevice->SetTexture(0, pTexture);
	// ポリゴン描画(頂点配列使用)
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vertex, sizeof(VERTEX_2D));
	// デバイス解放
//	SAFE_RELEASE(pDevice);
}
// 1なら正,-1なら逆向き
void CSprite::SetDirection(int direct) {
	m_direction = direct;
}
//-------関数----------

// セットテクスチャ関数
// 引数：テクスチャのパス
// 戻り値：LPDIRECT3DTEXTURE9型　テクスチャ
// テクスチャのパスを送るとテクスチャが返ってきます。
LPDIRECT3DTEXTURE9 SetTexture(LPCSTR pass) {
	// デバイス取得
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	LPDIRECT3DTEXTURE9 texture;
	if (FAILED(D3DXCreateTextureFromFile(pDevice, pass, &texture))) {
		MessageBox(NULL, "テクスチャ読み込み失敗", pass, MB_OK);
		texture = NULL;
	}
	// デバイス解放
	//SAFE_RELEASE(pDevice);
	pDevice = NULL;
	return texture;
}

// レンダーモード
void SetRenderMode(int mode) {
	// デバイス取得
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	switch (mode)
	{
	case		BLEND_NOBLEND:				// 通常（ブレンド無し）
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		break;
	case		BLEND_TRANSLUCENT:			// 半透明
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case		BLEND_ADDITION:				// 加算合成
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case		BLEND_ADDITION_TRANSLUCENT:	// 加算半透明合成
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case		BLEND_SUBTRACTION:			// 減算合成
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		break;
	case		BLEND_MULTIPLICATION:		// 乗算合成
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		break;
	case		BLEND_SCREEN:				// スクリーン合成
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case		BLEND_REVERSE:				// リバース
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		break;
	default:
		break;
	}
	// デバイス解放
	pDevice = NULL;
}