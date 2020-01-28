///////////////////////////////////////////////////
//
//		セレクトシーン　(select.h)
//
//////////////////////////////////////////////////
#ifndef _SELECT_H_
#define _SELECT_H_
//=============================================
//				インクルード
//=============================================
#include "main.h"
#include "scene_mgr.h"
#include "fade.h"
#include "sprite.h"
#include "game.h"
#include "character.h"
#include <vector>
using namespace std;

//=============================================
//				定数定義
//=============================================
#define MAX_SET_NUM (1 + MAX_TEAMMEMBER * 2)
#define MAX_VS_TYPE (4)
//=============================================
//				列挙体
//=============================================

// 画像
enum SELECT_TEXTURE
{
	//バック画面
	TEX_BACK = 0,

	//フォント
	TEX_SINGLE_FONT,
	TEX_MULTI_FONT,
	TEX_2P_FONT,
	TEX_3P_FONT,
	TEX_4P_FONT,
	TEX_2P_VS_FONT,
	TEX_3P_VS_FONT,
	TEX_4P_VS_FONT,

	//枠
	TEX_PICK,

	//フィール
	TEX_FIELD_ROOM,
	TEX_FIELD_STREET,

	//キャラクタ
	TEX_CHARA_LOUCUST,
	TEX_CHARA_FIREFLY,
	TEX_CHARA_BETTLE,

	//バケツ
	TEX_BUCKET_VERTICAL,
	TEX_BUCKET_HORIZONTAL,

	//次
	TEX_NEXT,

	//敵の数
	TEX_ENEMYTEAM,
	TEX_1TEAM,
	TEX_2TEAM,
	TEX_3TEAM,

	//プレイヤー番号
	TEX_PLAYER1,
	TEX_PLAYER2,
	TEX_PLAYER3,
	TEX_PLAYER4,

	//戻る
	TEX_RELEASE,

	//最大
	MAX_TEX,
};

//=============================================
//　モード種類
//=============================================
enum MODE
{
	MODE_SELECT = 0,
	MODE_SET_GAMEPARAM,
	MODE_SET_GAME,

	MAX_MODE,
};

//=============================================
//モードタイプ
//=============================================
enum MODE_TYPE
{
	TYPE_SINGLE = 0,
	TYPE_MULTI,

	MAX_MODE_TYPE,
};
//
//
//
enum PLAYER_NUM
{
	PLAYERNUM_2 = 0,
	PLAYERNUM_3,
	PLAYERNUM_4,
	MAX_PLAYERNUM,
};
//
//
//
enum VS_3P
{
	VS_1P_COM_COM = 0,
	VS_1P_2P_COM,
	VS_1P_2P_3P,

	MAX_3P_VS,
};
enum VS_4P
{
	VS_1P_COM_COM_COM = 0,
	VS_1P_2P_COM_COM,
	VS_1P_2P_3P_COM,
	VS_1P_2P_3P_4P,

	MAX_4P_VS,
};

//=============================================
//	選択状態
//=============================================
enum STATE
{
	STATE_OK = 0,
	STATE_YET,

	MAX_STATE,
};
//ゲーム情報選択
enum GAME_PARAM
{
	SELECT_STAGE = 0,
	SELECT_TEAMMEMBER,
	SELECT_USE_BUCKET,
	SELECT_NEXT,
	SELECT_BACK,
	MAX_SELECT_PARAM
};

//=============================================
//					構造体
//=============================================
typedef struct
{
	D3DXVECTOR2 pos;	//位置
	D3DXVECTOR2 size;	//大きさ
	int divideX;		//分割（横）
	int divideY;		//分割（縦）

}ST_BASICPARAM;
//=============================================
//					クラス
//=============================================
class CSelect :public CBaseScene, public CScene
{
public:
	CSelect();
	~CSelect();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:

	//デバイス取得
	LPDIRECT3DDEVICE9 m_pDevice;
	// 作成テクスチャ
	LPDIRECT3DTEXTURE9 m_pTexSplitMode[MAX_TEX];
	// 作成テクスチャサーフェイス
	LPDIRECT3DSURFACE9 m_pSurfaceSplitSelect;
	// 画面のバッファー
	LPDIRECT3DSURFACE9 m_pBackBuffer;

	//バック画面テクスチャ情報
	CSprite *m_pSpriteBack;
	CSprite *m_pSpritePick;

	CSprite *m_pSpriteModeTex[MAX_MODE];
	CSprite *m_pSpriteStage[MAX_STAGE_TYPE];
	CSprite *m_pSpritePlayerNum[MAX_PLAYERNUM];
	CSprite *m_pSprite2pVs;
	CSprite *m_pSprite3pVs[MAX_3P_VS];
	CSprite *m_pSprite4pVs[MAX_4P_VS];
	CSprite *m_pSpriteCharacter[MAX_TEAMMEMBER][MAX_CHARACTER_TYPE];
	CSprite *m_pSpriteBucket[MAX_TEAMMEMBER][MAX_BUCKET_TYPE];
	CSprite *m_pSpriteNext;
	CSprite *m_pSpriteEnemyNum[MAX_TEAM];
	CSprite *m_pSpritePlayerNo[MAX_TEAM];
	CSprite *m_pSpriteRelease;

	//バック画面情報
	ST_BASICPARAM m_backPrm;
	//モード選択画面情報
	ST_BASICPARAM m_modePrm[MAX_MODE];
	//ステージ情報
	ST_BASICPARAM m_stagePrm[MAX_STAGE_TYPE];
	//キャラクタ情報
	ST_BASICPARAM m_characterPrm[MAX_TEAMMEMBER][MAX_CHARACTER_TYPE];
	//バケツ情報
	ST_BASICPARAM m_bucketPrm[MAX_TEAMMEMBER][MAX_BUCKET_TYPE];
	//
	ST_BASICPARAM m_playerNumPrm[MAX_PLAYERNUM];
	//
	ST_BASICPARAM m_2pVsPrm;
	ST_BASICPARAM m_3pVsPrm[MAX_3P_VS];
	ST_BASICPARAM m_4pVsPrm[MAX_4P_VS];

	//選択枠情報
	ST_BASICPARAM m_pickPrm;
	//次ボタン情報
	ST_BASICPARAM m_nextPrm;

	//戻るボタン情報
	ST_BASICPARAM m_releasePrm;
	//敵の数画像情報
	ST_BASICPARAM m_enemyNumPrm[MAX_TEAM];

	//プレイヤー番号
	ST_BASICPARAM m_PlayerNoPrm[MAX_TEAM];

	//ゲーム情報格納
	ST_GAMEPARAM m_gameParam;

	//ステージの数
	int m_stageNo;

	//キャラクタoタイプ番号
	int m_charaNo[MAX_TEAMMEMBER * MAX_TEAM];

	//バケツタイプ番号
	int m_bucketNo[MAX_TEAMMEMBER *MAX_TEAM];

	//＝＝＝＝＝＝＝＝＝モード選択情報＝＝＝＝＝＝＝＝＝
	//
	bool m_setParamOK;
	//セレクト画面タイプ
	int m_ModeType;
	//モード番号 
	int m_modeNo;
	//フォントの拡大率カウント
	float m_scaleChange;
	//
	bool m_pushFlag;
	//
	int m_selectplayer;
	//
	int m_selectnow;
	//敵のチームの数
	int m_enemyNum;
	//敵の数の選択
	int m_selectEnemyNum;

	//////////////
	//
	int m_vsDivideNum;
	//
	int m_editVs;
	//
	int m_selectVsNow;

	//＝＝＝＝＝＝＝＝＝選択情報＝＝＝＝＝＝＝＝＝
	//
	int m_sumChara;
	//
	int m_playerNum;
	//
	int m_playerNo;
	//選択範囲
	int m_range;
	//選択中位置
	int m_pickNo;
	//選択フラグ
	int m_setStateFlag;
	//
	int m_rangeOld;
	//
	int m_setCharaFlag[MAX_TEAMMEMBER];
	//
	int m_setBucketFlag[MAX_TEAMMEMBER];
	//
	int m_checkParam;
	//＝＝＝＝＝＝＝＝＝キャラクタ選択情報＝＝＝＝＝＝＝＝＝

	//キャラクタタイプ
	int m_charaType;
	//編集番号
	int m_editChara;

	//＝＝＝＝＝＝＝＝＝バケツ選択情報＝＝＝＝＝＝＝＝＝

	//バケツのタイプ
	int m_bucketType;
	//選択中
	int m_editBucket;

	//========== プロトタイプ ==========
	void InitSelectInf();

	void SelectMode();

	void SetGamePrm();
	void StagePrm();
	void CharacterPrm();
	void BucketPrm();
	void DrawModeSelect();

	void LoadTexture();

};

#endif // !_SELECT_H_
