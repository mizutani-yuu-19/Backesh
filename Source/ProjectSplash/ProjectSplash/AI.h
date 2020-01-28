///////////////////////////////////////////////////
//
// キャラクター
//
//////////////////////////////////////////////////
#ifndef _AI_H_
#define _AI_H_
//----------------インクルード-------------------
#include "main.h"
#include "water_atk.h"
#include "game.h"
#include "scene_mgr.h"
#include "water_puddle.h"
#include "character.h"
//---------------定数マクロ------------

//-------------クラス----------------
class CAi :public CCharacter
{
public:
	CAi(int aiType, int teamNo, int bucketType);
	~CAi();

	void Init();
	void Uninit();
	void Update();
	void Draw(int camNo);

	void ChangeOccasion();
private:

	int					difficulty;					//難易度

	int					m_atkType;					//攻撃パターン２種	
	int					m_intelligence;				//AIの賢さ
	int					m_rotPattern;				//壁に当たった時の回転方向抽選用
	int					m_wallTime;					//壁を認識した時回転方向に角度を与えるカウント
	int					m_rotCount;					//壁を認識した時方向を変えるカウント
	int					m_resetCount;				//回転リセットカウント
	int					m_searchCount;				//最短距離にいる敵探索カウント
	int					m_targetNo;					//狙っている敵番号
	int					m_prevTargetNo;				//ひとつ前に狙っていた敵番号
	float				m_wallDistJump;				//ジャンプ可能な壁との最短距離

	D3DXVECTOR3			m_pPos;						//敵座標格納用
	D3DXVECTOR3			m_pRandPos;						//敵座標格納用
	D3DXVECTOR3			m_pPosTmp;					//敵座標格納用（一時的）
	D3DXVECTOR3			m_pDistVector;				//AIと敵の距離（ベクトル）
	D3DXVECTOR3			m_pDistVectorRand;			//AIとランダム座標の距離
	D3DXVECTOR3			m_pDistVectorTmp;			//AIと敵の距離（ベクトル）を一時的に格納
	D3DXVECTOR3			m_pRotEnemy;				//AIと敵の角度だけを見る
	D3DXVECTOR3			m_markUpPos;				//上に行くために目指す座標
	D3DXVECTOR3			m_markUpDistVector;			//上に行くために目指す座標との距離
	D3DXVECTOR3			m_upStairsPos;				//上（最終地点）座標
	D3DXVECTOR3			m_upStairsVector;			//上（最終地点）との距離
	D3DXVECTOR3			m_refugePos;				//HPが削れている状態での避難座標
	D3DXVECTOR3			m_refugeVector;				//HPが削れている状態での避難座標との距離

	D3DXVECTOR3			m_puddlePos;				//水たまりの座標格納用
	D3DXVECTOR3			m_puddleDistvector;			//AIと水たまりの距離格納用
	D3DXVECTOR3			m_puddleDistvectorMin;		//AIと水たまりの最短距離
	CWaterPuddle*		m_puddleDistMin;			// 一番近い水たまりのアドレス

	bool				m_markUP;					//上の地点を目指すための座標に向かう（階段とか坂とか）
	bool				m_upStairs;					//実際に上に行っている
	bool				m_randSearch;				//敵がサーチ範囲外の時、ランダム座標を探すフラグ
	bool				m_shiftPos;					//敵の座標を難易度に応じてずらす




	D3DXMATRIX			m_mtxWorld;					// ワールドマトリクス



	void DeathAi();									// キャラ死亡

	void Operate();									//操作全般

	bool				m_modeRotWall;				//現在のモード

};

#endif // !_AI_H_