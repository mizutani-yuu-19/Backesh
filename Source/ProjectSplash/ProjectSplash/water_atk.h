//=============================================================================
//
// モデル処理 [water_atk.h]
//
//=============================================================================
#ifndef _WATERATK_H_
#define _WATERATK_H_

#include "main.h"


//---------定数マクロ------------
#define WATER_SIZE				(2.0f)
#define WATER_MAX_COL			(750)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 move;			//速度
	D3DCOLOR	color;			//色
	D3DXVECTOR3 accel;			//加速度
	int			count;			//生存時間
	bool		bUse;			//使用しているかどうか
	int			delay;			// 少しずつ生成し飛ばす
	int			colGroup;		// 当たり判定グループ
}WATER_PARTICLE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CWaterAtk
{
public:
	CWaterAtk();
	CWaterAtk(D3DXVECTOR3 pos, float rot, float pow, int pat);
	~CWaterAtk();

	void Init(void);			//初期化処理
	void Uninit(void);			//終了処理
	bool Update(void);			//更新処理
	void Draw(int no);			//描画処理
	D3DXVECTOR3 GetPos(int no,int *num);				// 当たり判定noグループのパーティクルの位置とそのグループに含まれる衝突個数を返す
	int GetNumParticle();								// 発射数取得
	static void CreateBillboard();						// ビルボード作成とテクスチャ読み込み
	static void ReleaseBillboard();						// ビルボードとテクスチャ破棄
	void SetUseDown(int no);								// useフラグをおろす
private:
	int m_delay;										// 一気に出ないように
	int m_numParticle;
	bool m_puddleFlag[3];										// 床に水を作るフラグ
	D3DXVECTOR3	m_puddlePos[3];
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	int m_pat;						// 縦投げ、横投げ
	WATER_PARTICLE *m_particle;		// パーティクル情報
	WATER_PARTICLE*	m_firstParticle;		// パーティクル先頭アドレス
	std::vector<WATER_PARTICLE*> m_colList[WATER_MAX_COL];			// 整列されたパーティクル

	int cnt = 0;
	int particleNo[1000];

};

#endif
