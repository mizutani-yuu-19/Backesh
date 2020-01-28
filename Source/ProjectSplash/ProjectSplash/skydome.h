///////////////////////////////////////////////////
//
// スカイドーム
//
//////////////////////////////////////////////////
#ifndef _SKYDOME_H_
#define _SKYDOME_H_
//----------------インクルード-------------------
#include "main.h"
#include "game.h"
#include "model.h"
//---------------定数マクロ------------

// モデルファイル名
#define	MODEL_SKYDOME1			"data/MODEL/classroom.x"		// ステージ1
#define	MODEL_SKYDOME2			"data/MODEL/skydome.x"		// ステージ2

//-------------------列挙体定義----------------

//-------------クラス----------------
class CSkydome
{
public:
	CSkydome(int characterType);
	~CSkydome();

	void Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw(int camNo);

public:	// 静的
	static void CreateSkydome(int type);
	static CSkydome *m_skydome;				// 実体
	static void DeleteSkydome();
private:

	MODEL					m_model;

	D3DXMATRIX				m_mtxWorld;					// ワールドマトリクス

};

#endif // !_SKYDOME_H_