///////////////////////////////////////////////////
//
// ステージ
//
//////////////////////////////////////////////////
#ifndef _STAGE_H_
#define _STAGE_H_
//----------------インクルード-------------------
#include "main.h"
#include "game.h"
#include "model.h"
//---------------定数マクロ------------

// モデルファイル名
#define	MODEL_STAGE1			"data/MODEL/showstage.x"		// ステージ1
#define	MODEL_STAGE2			"data/MODEL/showstage2.x"		// ステージ2
#define	MODEL_COL_STAGE1		"data/MODEL/colstage.x"		// 当たり判定用ステージ1
#define	MODEL_COL_STAGE2		"data/MODEL/colstage2.x"		// 当たり判定用ステージ2
#define MAX_MISSING_POINT		(8)							//敵サーチミス時のランダム座標パターン数

//-------------------列挙体定義----------------


//-------------クラス----------------
class CStage
{
public:
	CStage(int characterType);
	~CStage();

	void Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw(int camNo);

	static D3DXVECTOR3 m_missingPoint[MAX_MISSING_POINT];
public:	// 静的
	static void CreateStage(int type);
	static CStage *m_stage;				// 実体
	void CStage::GetStageBound(D3DXVECTOR3 *max, D3DXVECTOR3 *min);	// ステージのサイズ
	D3DXVECTOR3 GetStageSize();

	LPD3DXMESH				m_pColStageMeshModel;			// メッシュ情報へのポインタ
private:
	// 衝突判定用モデル
	LPD3DXBUFFER			m_pColStageBuffMatModel;		// マテリアル情報へのポインタ
	DWORD					m_nColStageNumMatModel;			// マテリアル情報の数
															// 見た目モデル
	MODEL					m_showStageModel;			// メッシュ情報へのポインタ

	D3DXVECTOR3				m_size;						// 大きさ

	D3DXMATRIX				m_mtxWorld;					// ワールドマトリクス

	D3DXVECTOR3			m_max;
	D3DXVECTOR3			m_min;
};

#endif // !_STAGE_H_