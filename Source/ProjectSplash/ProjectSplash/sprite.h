#ifndef _SPRITE_H_
#define	_SPRITE_H_

#include "main.h"
//-----------定数マクロ-------------
enum {
	BLEND_NOBLEND,					// 通常（ブレンド無し）
	BLEND_TRANSLUCENT,				// 半透明
	BLEND_ADDITION,					// 加算合成
	BLEND_ADDITION_TRANSLUCENT,		// 加算半透明合成
	BLEND_SUBTRACTION,				// 減算合成
	BLEND_MULTIPLICATION,			// 乗算合成
	BLEND_SCREEN,					// スクリーン合成
	BLEND_REVERSE,					// リバース
};

class CSprite {
public:
	CSprite();
	~CSprite();
	void InitSprite(D3DXVECTOR2 pos, float width, float height, int divideX, int divideY);
	void DrawSprite(LPDIRECT3DTEXTURE9 pTexture);
	void SetDirection(int direct);		// 向き変更　１or-1

	D3DXVECTOR2 m_pos;			// 中心位置
	D3DXVECTOR2	m_scale;		// 拡大率
	float		m_angleDegree;	// 回転角度（°）
	D3DCOLOR	m_color;		// 色
	int			m_patNum;		// パターンNo
	float		m_width;		// スプライトの幅（テクスチャの切り取り幅）
	float		m_height;		// スプライトの高さ（テクスチャの切り取り高さ）
private:
	int			m_direction;	// 向き
	VERTEX_2D	m_vertex[4];	// 頂点情報
	int			m_divideX;		// 分割数X
	int			m_divideY;		// 分割数Y

protected:

};

//-------プロトタイプ---------
LPDIRECT3DTEXTURE9 SetTexture(LPCSTR pass);
void SetRenderMode(int mode);
#endif // !_SPRITE_H_