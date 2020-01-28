///////////////////////////////////////////////////
//
// キャラクター
//
//////////////////////////////////////////////////
#ifndef _CHARACTER_H_
#define _CHARACTER_H_
//----------------インクルード-------------------
#include "main.h"
#include "water_atk.h"
#include "game.h"
#include "scene_mgr.h"
#include "water_puddle.h"
#include "MeshObj.h"
#include "model.h"

//---------------定数マクロ------------
// モデルファイル名
#define	MODEL_CHARACTER_LOUCUST		("data/MODEL/loucust")	//バッタ
#define	MODEL_CHARACTER_FIREFLY		("data/MODEL/firefly")	//蛍
#define	MODEL_CHARACTER_BEETLE		("data/MODEL/beetle")	//カブトムシ

//------------列挙体定義---------------

// アニメーション列挙
enum {
	ANIM_PICKUP,		// 水をすくう
	ANIM_SIDESTEP,		// 横歩き
	ANIM_SPLASH,		// スプラッシュ
	ANIM_JUMP,			// ジャンプ
	ANIM_RUN,			// 走る
	ANIM_WAIT,			// 待機
	ANIM_MAX
};

//-------------クラス----------------
class CCharacter : public CMeshObj
{
public:
	CCharacter(int characterType, int teamNo, int bucketType);
	~CCharacter();

	virtual void Init();
	void Uninit();
	virtual void Update();
	virtual void Draw(int camNo);

	static void AllUpdate();
	static void AllDraw(int camNo);

	static int m_killAllCnt[MAX_TEAM];				// チームのトータルキルカウント
	static int m_deathAllCnt[MAX_TEAM];				// チームのトータルデスカウント


	static int			m_teamNum;					// チーム数
	static CCharacter*	m_character[MAX_TEAM][MAX_TEAMMEMBER];		//全キャラクターの配列

	static D3DXVECTOR3	GetPlayerPos(int teamNo);					// プレイヤーの座標を返す
	static D3DXVECTOR3	GetPlayerMove(int teamNo);					// プレイヤーの移動量を返す
	static float		GetPlayerRot(int teamNo);					// プレイヤーの回転方向
	static float		GetPlayerWaterVol(int teamNo);				// プレイヤーの水残量
	static float		GetPlayerLife(int teamNo);					// プレイヤーの水残量
	static D3DXVECTOR3	GetPlayerKillerPos(int teamNo);				// プレイヤーを殺したやつの位置
	static bool			GetPlayerSurvive(int teamNo);				// プレイヤーが生きてる
	static float		GetPlayerSkillGauge(int teamNo);				// プレイヤーのスキルゲージ

	static CCharacter*	changeFrom;									// プレイヤーチェンジ元
	static CCharacter*	changeTo;									// プレイヤーチェンジ先
	static int			m_contentPos[MAX_TEAM][MAX_TEAMMEMBER];				// 配列交換時の中身がどこに行ったか
	static void			ChangePlayer();								// キャラチェンジ
	static int			GetCharacterContentPos(int teamNo, int memberNo) { return m_contentPos[teamNo][memberNo]; }
	//	static	bool		GetCharacterInvincible(int teamNo, int memberNo) { return m_character[teamNo][memberNo]; }				// プレイヤーのスキルゲージ

		// キャラクター作成
		// 引数１キャラの種類の配列の先頭アドレス
		// 引数２プレイヤーの人数
		// 引数３チームの数
	static void CreateCharacter(int *type, int, int, int *bucketType);
	static void LoadCharacterModel();					//モデルとテクスチャをロード
	static void ReleaseCharacterModel();					//モデルとテクスチャをリリース

	// キャラクターのタイプを返す
	static int GetCharacterType(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_type; };
	static float GetCharacterLife(int teamNo, int memberNo) {
		if (m_character[teamNo][memberNo]->m_life < 0.0f)	return 0.0f;
		return (float)(m_character[teamNo][memberNo]->m_life / (float)m_character[teamNo][memberNo]->m_maxLife);
	};
	static bool GetCharacterSurvive(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_surviveFlag; }
	static int GetCharacterInvincible(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_invincibleTime; }
	static CWaterAtk* GetCharacterWater(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_water; }
	//static bool GetCharacterSkillFlag(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_skillFlag; }
	// キャラクターの位置を返す
	D3DXVECTOR3 GetCharacterPos(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_afterPos; };
	// キル数を返す
	static int GetKillCount(int teamNo) { return m_killAllCnt[teamNo]; }
	static void CharacterKill(int teamNo, int memberNo, int killerNo) { m_character[teamNo][memberNo]->Kill(killerNo); }
	void Kill(int killerNo);

	static int m_winner;
	static void ResultFunc();
	static int GetWinner();
protected:
	D3DXVECTOR3			m_beforePos;				//現在の位置
	D3DXVECTOR3			m_afterPos;					//現在の位置
	float				m_moveSpeed;				//移動スピード
	float				m_maxMoveSpeed;				//移動スピード
	D3DXVECTOR3			m_rot;						//現在の向き
	D3DXVECTOR3			m_size;						//大きさ
	int					m_teamNo;					//チームナンバー
	int					m_type;						//キャラクターのタイプ

	int					m_No;						//チーム内での番号
	int					m_life;						//今の体力
	int					m_maxLife;					//最大体力
	bool				m_jumpFlag;					//ジャンプフラグ
	bool				m_playerFlag;				//プレイヤーフラグ

	bool				m_surviveFlag;				//生存フラグ
	int					m_killer;					//誰に殺された
	int					m_respawnTime;				//生き返る時間

	float				m_avoidCoolTime;			// 回避クールタイム
	float				m_invincibleTime;			// 無敵時間

	int m_killCnt;									// 自分のキルカウント
	int m_deathCnt;									// 自分のデスカウント

	D3DXVECTOR3			m_move;						//移動量
	D3DXVECTOR3			m_accel;					//加速量

	float				m_floorDist;				// 床までの距離
	float				m_wallDistFront;			// 壁までの距離

	D3DXMATRIX			m_mtxWorld;					//ワールドマトリクス

	bool				m_sideStepFlag;				// 横歩きフラグ
	bool				m_splashFlag;				// 横歩きフラグ

	// 水関係
	float				m_waterVolume;		// 水の量
	float				m_pickUpVolume;		// すくう水の量
	int					m_pickUpTime;		// 水をすくう残り時間
	int					m_pickUpHandle;		// すくう水たまりのハンドル
	CWaterAtk			*m_water;			// バケツクラス
	int					m_bucketType;		// バケツのタイプ
	void				PickUpWater();		// 水をすくう関数
	static MODEL		m_bucketModel[2];	// バケツのモデル

	// 必殺技
	float				m_skillGauge;		// 必殺技のゲージ最大１．０ｆ
	bool				m_skillFlag;		// 必殺技使用中
	float				m_skillAccumulate;	// ゲージ回復量

	static LPDIRECT3DTEXTURE9		m_pD3DTextureLockOn;		// テクスチャへのポインタ
public:
	void DamageLife();							//被弾ダメージ
	bool RespawnCharacter();					//リスポーン処理
	void KillCountAdd();						// キルカウントを増やす。倒したときに呼んでね
	void DeathCountAdd();						// デスカウントを増やす。死んだときに呼んでね
private:
	D3DXVECTOR3 m_respawnPos;

	int m_healCnt;								// 回復する間隔
	float m_walkSECnt;							// 足音を出すカウント

	void UpdatePickUp();
};

#endif // !_CHARACTER_H_