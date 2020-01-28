//=============================================================================
//
// 水たまり処理 [water_puddle.h]
//
//=============================================================================
#ifndef _WATER_PUDDLE_H_
#define _WATER_PUDDLE_H_

#include "main.h"
//-----------定数マクロ----------
#define PUDDLE_SIZE				(2.0f)
//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CWaterPuddle
{
public:
	CWaterPuddle(D3DXVECTOR3 pos, float vol, bool infinite);
	~CWaterPuddle();

	void Init(void);			//初期化処理
	void Uninit(void);			//終了処理
	void Update(void);					//更新処理
	void Draw(void);					//描画処理
	D3DXVECTOR3			m_pos;					// 位置
	D3DXVECTOR3			m_size;					// 大きさ(モデルサイズと掛け合わせて)
	static D3DXVECTOR3	GetWaterPuddlePos(int no);	//水たまりの座標を取得
	static D3DXVECTOR3	GetWaterPuddleSize(int no);	//水たまりの大きさを取得
	static int			GetWaterHandle(int no);			// 水たまりのハンドル取得
	static float		GetWaterPuddleVolume(int no);	//水たまりの水量

	static void CreateWaterPuddle(D3DXVECTOR3 pos, float vol, bool infinite);	// 水たまり生成
	static void LoadModel();											// 水たまりのモデルロード
	static void ReleaseModel();											// 水たまりのモデル開放
	static void UpdateWaterPuddle();									// 水たまりの全更新
	static void DrawWaterPuddle();										// 水たまりの全描画
	static void SubRemain(int no);										// 水量を減らす（指定）	
	static void SubRemain2(int handle, float volume);									// 水量を減らす（指定）	
	void Absorb();														// 水量を減らす
	void Absorb(float vol);														// 水量を減らす

	static std::vector<CWaterPuddle*>	m_waterPuddle;

	static int			NoCnt;					// 一意な番号
	int					m_handle;					// 何体目			一時的にpublicに
private:
	D3DXMATRIX			m_mtxWorld;				// ワールドマトリクス
	float				m_remain;				// 残り水量
	bool				m_infiniteFlag;			// 無限に水があるフラグ
	static D3DXVECTOR3			m_modelSize;
};


#endif
