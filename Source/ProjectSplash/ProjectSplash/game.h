///////////////////////////////////////////////////
//
// ゲームシーン
//
//////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_
//----------------インクルード-------------------
#include "main.h"
#include "scene_mgr.h"

//=============================================
//			定 数 定 義
//=============================================
#define MAX_MODEL		(10)				//最大モデル数
#define	MAX_TEAM		(4)					//最大チーム数
#define MAX_TEAMMEMBER	(3)					//最大チーム内人数
#define GAME_TIME		(3.0f*3600)			//ゲームの時間３分

#define PLAYER_COLOR1	(D3DCOLOR_ARGB(255, 0, 0, 255))
#define PLAYER_COLOR2	(D3DCOLOR_ARGB(255, 255, 0, 0))
#define PLAYER_COLOR3	(D3DCOLOR_ARGB(255, 0, 255, 0))
#define PLAYER_COLOR4	(D3DCOLOR_ARGB(255, 255, 255, 0))
//=============================================
//			 列 挙 体 定 義
//=============================================

//　キャラクタの種類
enum CHARACTER_TYPE
{
	CHARACTER_TYPE_LOCUST = 0,	// バッタ
	CHARACTER_TYPE_FIREFLY,		// 蛍
	CHARACTER_TYPE_BETTLE,		//カブトムシ

	MAX_CHARACTER_TYPE	//キャラクタ数
};
//ステージの種類
enum STAGE_TYPE
{
	TYPE_ROOM = 0,	// 教室
	TYPE_ROAD,		// 路地

	MAX_STAGE_TYPE		//ステージの最大数
};
//チームの種類
enum TEAM_TYPE
{
	TEAM_TYPE_P1 = 0,	//プレイヤー１
	TEAM_TYPE_P2,		//プレイヤー２
	TEAM_TYPE_COM,		//コンピューター

	MAX_TEAM_TYPE

};
//バケツの種類
enum BUCKET_TYPE
{
	BUCKET_VERTICAL = 0,
	BUCKET_HORIZONTAL,

	MAX_BUCKET_TYPE
};

//ステップ
typedef enum {
	STEP_STAGEVIEW,
	STEP_CHARACTERVIEW,
	STEP_COUNTDOWN,
	STEP_GAME,
	STEP_FINISH,
}EN_STEP;

//=============================================
//　構 造 体 宣 言
//=============================================
typedef struct
{
	int characterType[MAX_TEAMMEMBER*MAX_TEAM];	//キャラクタータイプ
	int bucketType[MAX_TEAMMEMBER*MAX_TEAM];	//バケツのタイプ
	int stageType;		//ステージタイプ
	int teamNum;		//チーム数
	int playerNum;		//プレイヤー数
}ST_GAMEPARAM;
//-------------クラス----------------
class CGame :public CBaseScene
{
public:
	CGame();
	~CGame();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	static void SetGameParam(ST_GAMEPARAM);
	static ST_GAMEPARAM GetGameParam();
	static int	GetTime();

	static int m_step;
	static int GetStep() { return m_step; }
	static void SetStep(int step) { m_step = step; }
private:
	static ST_GAMEPARAM m_gameParam;

	static int m_gameTime;
};

#endif // !_GAME_H_