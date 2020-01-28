///////////////////////////////////////////////////
//
// セレクトシーン
//
//////////////////////////////////////////////////

//----------------インクルード--------------
#include "select.h"
#include "debugproc.h"
#include "game_pad.h"
#include "input.h"
#include "character.h"

//---------------定数定義---------------

#define BACK_POS_X (SCREEN_CENTER_X)
#define BACK_POS_Y (SCREEN_CENTER_Y)

#define BACK_SIZE_X (SCREEN_WIDTH)
#define BACK_SIZE_Y (SCREEN_HEIGHT)

#define MODE_FONT_SIZE_X	(400.0f)
#define MODE_FONT_SIZE_Y	(100.0f)

//選択テクスチャファイル名
#define TEX_BACKGROUND_PATH		("data/TEXTURE/title/background.png")
#define TEX_SINGLE_PATH			("data/TEXTURE/select/single.png")
#define TEX_MULTI_PATH			("data/TEXTURE/select/multi.png")
#define TEX_2P_PATH				("data/TEXTURE/select/2P.png")
#define TEX_3P_PATH				("data/TEXTURE/select/3P.png")
#define TEX_4P_PATH				("data/TEXTURE/select/4P.png")
#define TEX_VS_2P_PATH			("data/TEXTURE/select/2P_set.png")
#define TEX_VS_3P_PATH			("data/TEXTURE/select/3P_set.png")
#define TEX_VS_4P_PATH			("data/TEXTURE/select/4P_set.png")

//ステージテクスチャファイル名
#define TEX_STAGE_ROOM_PATH			("data/TEXTURE/select/field000.png")
#define TEX_STAGE_ROAD_PATH			("data/TEXTURE/select/field001.png")

//キャラクタファイル名
#define TEX_CHARACTER_LOCUST_PATH	("data/TEXTURE/select/character000.png")
#define TEX_CHARACTER_FIREFLY_PATH	("data/TEXTURE/select/character001.png")
#define TEX_CHARACTER_BETTLE_PATH	("data/TEXTURE/select/character002.png")

//バケツテクスチャファイル名
#define TEX_BUCKET_VERTICAL_PATH	("data/TEXTURE/select/bucket000.png")
#define TEX_BUCKET_HORIZONTAL_PATH	("data/TEXTURE/select/bucket001.png")

//選択フレームファイル名
#define TEX_PICK_PATH				("data/TEXTURE/select/selectframe.png")

//状態テクスチャファイル名
#define TEX_OK_PATH					("data/TEXTURE/select/ok.png")
#define TEX_YET_PATH				("data/TEXTURE/select/yet.png")
//次へテクスチャファイル名
#define TEX_NEXT_PATH				("data/TEXTURE/select/next.png")

//敵のテクスチャファイル名
#define TEX_ENEMYTEAM_PATH			("data/TEXTURE/select/enemyteam.png")
#define TEX_1TEAM_PATH				("data/TEXTURE/select/1team.png")
#define TEX_2TEAM_PATH				("data/TEXTURE/select/2team.png")
#define TEX_3TEAM_PATH				("data/TEXTURE/select/3team.png")

//プレイヤー番号ファイル名
#define TEX_PLAYER1_PATH			("data/TEXTURE/select/no.png")
#define TEX_PLAYER2_PATH			("data/TEXTURE/select/no1.png")
#define TEX_PLAYER3_PATH			("data/TEXTURE/select/no2.png")
#define TEX_PLAYER4_PATH			("data/TEXTURE/select/no3.png")

//戻る
#define TEX_RELEASE_PATH			("data/TEXTURE/select/back.png")

//---------------メンバ関数---------------
bool g_change = false;
//==============================================
//			コ　ン　ス　ト　ラ　ク　タ
//==============================================
CSelect::CSelect()
{
	//デバイスの取得
	m_pDevice = GetDevice();
	//テクスチャの読み込み
	LoadTexture();
	//バック画面テクスチャ生成
	m_pSpriteBack = new CSprite();
	//モード選択情報
	m_ModeType = TYPE_SINGLE;
	m_modeNo = MODE_SELECT;
	m_scaleChange = 0.0f;
	m_pushFlag = false;
	m_selectplayer = 0;
	m_selectnow = 0;
	m_vsDivideNum = 0;
	m_selectVsNow = 0;
	m_editVs = 0;

	//モードテクスチャ生成
	for (int i = 0; i < MAX_MODE_TYPE; i++)
		m_pSpriteModeTex[i] = new CSprite();
	//選択情報
	m_range = 0;
	m_pickNo = 0;
	m_rangeOld = 0;
	m_checkParam = 0;
	for (int i = 0; i < MAX_PLAYERNUM; i++)
		m_pSpritePlayerNum[i] = new CSprite();
	m_pSprite2pVs = new CSprite;

	for (int i = 0; i < MAX_3P_VS; i++)
		m_pSprite3pVs[i] = new CSprite;
	for (int i = 0; i < MAX_4P_VS; i++)
		m_pSprite4pVs[i] = new CSprite;
	//枠テクスチャ生成
	m_pSpritePick = new CSprite();
	//ステージテクスチャ生成　
	for (int i = 0; i < MAX_STAGE_TYPE; i++)
	{
		m_pSpriteStage[i] = new CSprite();
	}
	m_stageNo = -1;
	//キャラクタ選択情報
	m_charaType = 0;
	m_setStateFlag = 0;
	m_editChara = 0;
	m_sumChara = 0;
	m_playerNum = 0;
	m_playerNo = 1;
	//バケツ選択情報
	m_bucketType = 0;
	m_editBucket = 0;
	//キャラクタ・バケツテクスチャ生成
	for (int j = 0; j < MAX_TEAMMEMBER; j++)
	{
		//キャラクタ生成
		for (int i = 0; i < MAX_CHARACTER_TYPE; i++)
			m_pSpriteCharacter[j][i] = new CSprite();
		//バケツ生成
		for (int i = 0; i < MAX_BUCKET_TYPE; i++)
			m_pSpriteBucket[j][i] = new CSprite();
	}
	//ゲーム画面情報
	m_gameParam.playerNum = 0;
	m_gameParam.stageType = 0;
	m_gameParam.teamNum = 0;
	//ゲーム用キャラクタタイプ格納場所の初期化
	for (int i = 0; i < MAX_TEAMMEMBER * MAX_TEAM; i++)
	{
		m_charaNo[i] = -1;
		if (i > MAX_TEAMMEMBER)continue;
		m_setCharaFlag[i] = -1;
	}
	//ゲーム用バケツタイプ格納場所の初期化
	for (int i = 0; i < MAX_TEAMMEMBER* MAX_TEAM; i++)
	{
		m_bucketNo[i] = -1;
		if (i > MAX_TEAMMEMBER)continue;
		m_setBucketFlag[i] = -1;
	}
	for (int i = 0; i < MAX_TEAMMEMBER * MAX_TEAM; i++)
	{
		m_gameParam.bucketType[i] = 0;
		m_gameParam.characterType[i] = 0;
	}
	m_pSpriteNext = new CSprite();
	//敵の数の画像用
	for (int i = 0; i < MAX_TEAM; i++)
		m_pSpriteEnemyNum[i] = new CSprite();
	//プレイヤーの番号用
	for (int i = 0; i < MAX_TEAM; i++)
		m_pSpritePlayerNo[i] = new CSprite();
	m_pSpriteRelease = new CSprite();

	m_selectEnemyNum = 1;
	m_enemyNum = 1;
}

//==============================================
//			デ　ス　ト　ラ　ク　タ
//==============================================
CSelect::~CSelect()
{

}

//==============================================
//			初　期　化　処　理
//==============================================
void CSelect::Init()
{
	InitSelectInf();

	StartFadeIn();
}

//==============================================
//			終　了　処　理
//==============================================
void CSelect::Uninit()
{
	if (m_pSpriteBack)
	{
		delete m_pSpriteBack;
		m_pSpriteBack = NULL;
	}

	if (m_pSpritePick)
	{
		delete m_pSpritePick;
		m_pSpritePick = NULL;
	}

	for (int i = 0; i < MAX_MODE_TYPE; i++)
	{
		if (m_pSpriteModeTex[i])
		{
			delete m_pSpriteModeTex[i];
			m_pSpriteModeTex[i] = NULL;
		}
	}

	for (int i = 0; i < MAX_PLAYERNUM; i++)
	{
		if (m_pSpritePlayerNum[i])
		{
			delete m_pSpritePlayerNum[i];
			m_pSpritePlayerNum[i] = NULL;
		}
	}

	if (m_pSprite2pVs)
	{
		delete m_pSprite2pVs;
		m_pSprite2pVs = NULL;
	}
	for (int i = 0; i < MAX_3P_VS; i++)
		if (m_pSprite3pVs[i])
		{
			delete m_pSprite3pVs[i];
			m_pSprite3pVs[i] = NULL;
		}
	for (int i = 0; i < MAX_4P_VS; i++)
		if (m_pSprite4pVs[i])
		{
			delete m_pSprite4pVs[i];
			m_pSprite4pVs[i] = NULL;
		}

	for (int i = 0; i < MAX_STAGE_TYPE; i++)
	{
		if (m_pSpriteStage[i])
		{
			delete m_pSpriteStage[i];
			m_pSpriteStage[i] = NULL;
		}
	}
	for (int j = 0; j < MAX_TEAMMEMBER; j++)
	{
		for (int i = 0; i < MAX_CHARACTER_TYPE; i++)
		{
			if (m_pSpriteCharacter[j][i])
			{
				delete m_pSpriteCharacter[j][i];
				m_pSpriteCharacter[j][i] = NULL;
			}
		}
		for (int i = 0; i < MAX_BUCKET_TYPE; i++)
		{
			if (m_pSpriteBucket[j][i])
			{
				delete m_pSpriteBucket[j][i];
				m_pSpriteBucket[j][i] = NULL;
			}
		}
	}

	if (m_pSpriteNext)
	{
		delete m_pSpriteNext;
		m_pSpriteNext = NULL;
	}

	for (int i = 0; i < MAX_TEAM; i++)
	{
		if (m_pSpriteEnemyNum[i])
		{
			delete m_pSpriteEnemyNum[i];
			m_pSpriteEnemyNum[i] = NULL;
		}
	}

	for (int i = 0; i < MAX_TEAM; i++)
	{
		if (m_pSpritePlayerNo[i])
		{
			delete m_pSpritePlayerNo[i];
			m_pSpritePlayerNo[i] = NULL;
		}
	}
	if (m_pSpriteRelease)
	{
		delete m_pSpriteRelease;
		m_pSpriteRelease = NULL;
	}
	for (int i = 0; i < MAX_TEX; i++)
		SAFE_RELEASE(m_pTexSplitMode[i]);

	SAFE_RELEASE(m_pSurfaceSplitSelect);
}

//==============================================
//			更　新　処　理
//==============================================
void CSelect::Update()
{
	if (EndFadeIn())
	{
		switch (m_modeNo)
		{
		case MODE_SELECT:
			SelectMode();
			break;
		case MODE_SET_GAMEPARAM:
			SetGamePrm();
			break;
		case MODE_SET_GAME:
			StartFadeOut();
			break;
		}
	}

	if ((GetPadButton(0, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN)) && m_modeNo == MODE_SELECT && !m_pushFlag && m_setParamOK)
	{

		StartFadeOut();
		m_pushFlag = true;
	}

	if (EndFadeOut() && m_modeNo == MODE_SELECT)
	{
		m_modeNo = MODE_SET_GAMEPARAM;
		StartFadeIn();
	}
	if (EndFadeOut() && m_modeNo == MODE_SET_GAME)
	{
		CGame::SetGameParam(m_gameParam);
		g_pScene->ChangeScene(SCENE_GAME);
	}
}

//==============================================
//			描　画　処　理
//==============================================
void CSelect::Draw()
{
	DrawModeSelect();
}

//==============================================
//	モード情報の初期化処理
//==============================================
void CSelect::InitSelectInf()
{
	//バック情報
	m_backPrm.pos = D3DXVECTOR2(BACK_POS_X, BACK_POS_Y);
	m_backPrm.size = D3DXVECTOR2(BACK_SIZE_X, BACK_SIZE_Y);
	m_backPrm.divideX = 1;
	m_backPrm.divideY = 1;
	m_pSpriteBack->InitSprite(m_backPrm.pos, m_backPrm.size.x, m_backPrm.size.y, m_backPrm.divideX, m_backPrm.divideY);

	//モード情報
	for (int i = 0; i < MAX_MODE_TYPE; i++)
	{
		m_modePrm[i].size = D3DXVECTOR2(MODE_FONT_SIZE_X, MODE_FONT_SIZE_Y);
		m_modePrm[i].pos.x = SCREEN_CENTER_X;
		m_modePrm[i].pos.y = (i * MODE_FONT_SIZE_Y) + SCREEN_CENTER_Y - MODE_FONT_SIZE_Y;
		m_modePrm[i].divideX = 1;
		m_modePrm[i].divideY = 1;
		m_pSpriteModeTex[i]->InitSprite(m_modePrm[i].pos, m_modePrm[i].size.x, m_modePrm[i].size.y, m_modePrm[i].divideX, m_modePrm[i].divideY);
	}
	//
	for (int i = 0; i < MAX_PLAYERNUM; i++)
	{
		m_playerNumPrm[i].size.x = 350.0f;
		m_playerNumPrm[i].size.y = 100.0f;
		m_playerNumPrm[i].pos = D3DXVECTOR2(300.0f, 200.0f + (m_playerNumPrm[i].size.y * i));
		m_playerNumPrm[i].divideX = 1;
		m_playerNumPrm[i].divideY = 1;

		m_pSpritePlayerNum[i]->InitSprite(m_playerNumPrm[i].pos, m_playerNumPrm[i].size.x, m_playerNumPrm[i].size.y, m_playerNumPrm[i].divideX, m_playerNumPrm[i].divideY);
		m_pSpritePlayerNum[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
	}
	//
	m_2pVsPrm.size = D3DXVECTOR2(300.0f, 100.0f);
	m_2pVsPrm.pos = D3DXVECTOR2(m_playerNumPrm[PLAYERNUM_2].pos.x + m_playerNumPrm[PLAYERNUM_2].size.x + 200.0f, m_playerNumPrm[PLAYERNUM_2].pos.y);
	m_2pVsPrm.divideX = 1;
	m_2pVsPrm.divideY = 1;
	m_pSprite2pVs->InitSprite(m_2pVsPrm.pos, m_2pVsPrm.size.x, m_2pVsPrm.size.y, m_2pVsPrm.divideX, m_2pVsPrm.divideY);
	m_pSprite2pVs->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);

	for (int i = 0; i < MAX_3P_VS; i++)
	{
		m_3pVsPrm[i].size = D3DXVECTOR2(400.0f, 100.0f);
		m_3pVsPrm[i].pos = D3DXVECTOR2(m_playerNumPrm[PLAYERNUM_3].pos.x + m_playerNumPrm[PLAYERNUM_3].size.x + 200.0f, m_playerNumPrm[PLAYERNUM_3].pos.y + m_3pVsPrm[i].size.y * i);
		m_3pVsPrm[i].divideX = 1;
		m_3pVsPrm[i].divideY = 3;
		m_pSprite3pVs[i]->InitSprite(m_3pVsPrm[i].pos, m_3pVsPrm[i].size.x, m_3pVsPrm[i].size.y, m_3pVsPrm[i].divideX, m_3pVsPrm[i].divideY);
		m_pSprite3pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
		m_pSprite3pVs[i]->m_patNum = i;
	}
	for (int i = 0; i < MAX_4P_VS; i++)
	{
		m_4pVsPrm[i].size = D3DXVECTOR2(600.0f, 100.0f);
		m_4pVsPrm[i].pos = D3DXVECTOR2(m_playerNumPrm[PLAYERNUM_4].pos.x + m_playerNumPrm[PLAYERNUM_4].size.x + 200.0f, m_playerNumPrm[PLAYERNUM_4].pos.y + m_4pVsPrm[i].size.y * i);
		m_4pVsPrm[i].divideX = 1;
		m_4pVsPrm[i].divideY = 4;
		m_pSprite4pVs[i]->InitSprite(m_4pVsPrm[i].pos, m_4pVsPrm[i].size.x, m_4pVsPrm[i].size.y, m_4pVsPrm[i].divideX, m_4pVsPrm[i].divideY);
		m_pSprite4pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
		m_pSprite4pVs[i]->m_patNum = i;
	}
	//ゲーム情報
	//ステージ
	for (int i = 0; i < MAX_STAGE_TYPE; i++)
	{
		m_stagePrm[i].size = D3DXVECTOR2(150.0f, 100.0f);
		m_stagePrm[i].pos = D3DXVECTOR2((i * 200.0f) + SCREEN_CENTER_X - 150.0f, (int)(SCREEN_HEIGHT * 0.15f));
		m_stagePrm[i].divideX = 1;
		m_stagePrm[i].divideY = 1;
		m_pSpriteStage[i]->InitSprite(m_stagePrm[i].pos, m_stagePrm[i].size.x, m_stagePrm[i].size.y, m_stagePrm[i].divideX, m_stagePrm[i].divideY);
	}
	//メンバー
	for (int j = 0; j < MAX_TEAMMEMBER; j++)
	{
		//キャラクタ
		for (int i = 0; i < MAX_CHARACTER_TYPE; i++)
		{
			m_characterPrm[j][i].size = D3DXVECTOR2(100.0f, 230.0f);
			m_characterPrm[j][i].pos = D3DXVECTOR2(j * (m_characterPrm[j][0].size.x + 100.0f) + m_stagePrm[0].pos.x - 100.0f,
				(m_stagePrm[0].pos.y + m_stagePrm[0].size.y + 100.0f));
			m_characterPrm[j][i].divideX = 1;
			m_characterPrm[j][i].divideY = 1;
			m_pSpriteCharacter[j][i]->InitSprite(m_characterPrm[j][i].pos, m_characterPrm[j][i].size.x, m_characterPrm[j][i].size.y, m_characterPrm[j][i].divideX, m_characterPrm[j][i].divideY);
		}

		//バケツ
		for (int i = 0; i < MAX_BUCKET_TYPE; i++)
		{
			m_bucketPrm[j][i].size = D3DXVECTOR2(100.0f, 200.0f);
			m_bucketPrm[j][i].pos = D3DXVECTOR2(m_characterPrm[j][0].pos.x,
				m_characterPrm[0][0].pos.y + m_bucketPrm[0][0].size.y + 50.0f);
			m_bucketPrm[j][i].divideX = 1;
			m_bucketPrm[j][i].divideY = 1;
			m_pSpriteBucket[j][i]->InitSprite(m_bucketPrm[j][i].pos, m_bucketPrm[j][i].size.x, m_bucketPrm[j][i].size.y, m_bucketPrm[j][i].divideX, m_bucketPrm[j][i].divideY);
		}
	}

	//状態

	//枠
	m_pickPrm.pos = D3DXVECTOR2(m_stagePrm[0].pos.x, m_stagePrm[0].pos.y);
	m_pickPrm.size = D3DXVECTOR2(20.0f + m_stagePrm[0].size.x, 20.0f + m_stagePrm[0].size.y);
	m_pickPrm.divideX = 1;
	m_pickPrm.divideY = 1;
	m_pSpritePick->InitSprite(m_pickPrm.pos, m_pickPrm.size.x, m_pickPrm.size.y, m_pickPrm.divideX, m_pickPrm.divideY);
	//次へボタン
	m_nextPrm.size = D3DXVECTOR2(100.0f, 50.0f);
	m_nextPrm.pos = D3DXVECTOR2(SCREEN_WIDTH - (m_nextPrm.size.x + 40.0f), SCREEN_HEIGHT - (m_nextPrm.size.y + 20.0f));
	m_nextPrm.divideX = 1;
	m_nextPrm.divideY = 1;
	m_pSpriteNext->InitSprite(m_nextPrm.pos, m_nextPrm.size.x, m_nextPrm.size.y, m_nextPrm.divideX, m_nextPrm.divideY);

	//敵のチーム数
	for (int i = 0; i < MAX_TEAM; i++)
	{
		if (i < 1)
		{
			m_enemyNumPrm[i].size = D3DXVECTOR2(600.0f, 100.0f);
			m_enemyNumPrm[i].pos = D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y - SCREEN_CENTER_Y * 0.7f);
		}
		else
		{
			m_enemyNumPrm[i].size = D3DXVECTOR2(300.0f, 100.0f);
			m_enemyNumPrm[i].pos = D3DXVECTOR2(SCREEN_CENTER_X - SCREEN_CENTER_X * 0.25f, SCREEN_CENTER_Y - SCREEN_CENTER_Y * 0.5f + (i - 1) * m_enemyNumPrm[i].size.y + 50.0f);
		}
		m_enemyNumPrm[i].divideX = 1;
		m_enemyNumPrm[i].divideY = 1;
		m_pSpriteEnemyNum[i]->InitSprite(m_enemyNumPrm[i].pos, m_enemyNumPrm[i].size.x, m_enemyNumPrm[i].size.y, m_enemyNumPrm[i].divideX, m_enemyNumPrm[i].divideY);
		m_pSpriteEnemyNum[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
	}

	//プレイヤー番号
	for (int i = 0; i < MAX_TEAM; i++)
	{
		m_PlayerNoPrm[i].size = D3DXVECTOR2(200.0f, 200.0f);
		m_PlayerNoPrm[i].pos = D3DXVECTOR2(SCREEN_CENTER_X - SCREEN_CENTER_X * 0.7f, SCREEN_CENTER_Y - SCREEN_CENTER_Y * 0.7f);
		m_PlayerNoPrm[i].divideX = 1;
		m_PlayerNoPrm[i].divideY = 1;
		m_pSpritePlayerNo[i]->InitSprite(m_PlayerNoPrm[i].pos, m_PlayerNoPrm[i].size.x, m_PlayerNoPrm[i].size.y, m_PlayerNoPrm[i].divideX, m_PlayerNoPrm[i].divideY);
		m_pSpritePlayerNo[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
	}

	//戻るボタン
	m_releasePrm.size = D3DXVECTOR2(100.0f, 50.0f);
	m_releasePrm.pos = D3DXVECTOR2(m_releasePrm.size.x + 10.0f, m_releasePrm.size.y + 20.0f);
	m_releasePrm.divideX = 1;
	m_releasePrm.divideY = 1;
	m_pSpriteRelease->InitSprite(m_releasePrm.pos, m_releasePrm.size.x, m_releasePrm.size.y, m_releasePrm.divideX, m_releasePrm.divideY);


}

//==============================================
//		モードの描画内容
//==============================================
void CSelect::DrawModeSelect()
{
	//バック画面の描画
	m_pSpriteBack->DrawSprite(m_pTexSplitMode[TEX_BACK]);

	//モード選択別分岐
	switch (m_modeNo)
	{//選択モード
	case MODE_SELECT:
		for (int i = 0; i < MAX_MODE_TYPE; i++)
		{
			switch (i)
			{
			case 0:
				m_pSpriteModeTex[i]->DrawSprite(m_pTexSplitMode[TEX_SINGLE_FONT]);
				break;
			case 1:
				m_pSpriteModeTex[i]->DrawSprite(m_pTexSplitMode[TEX_MULTI_FONT]);
				break;
			}
		}
		for (int i = 0; i < MAX_PLAYERNUM; i++)
		{
			switch (i)
			{
			case PLAYERNUM_2:
				m_pSpritePlayerNum[i]->DrawSprite(m_pTexSplitMode[TEX_2P_FONT]);
				break;
			case PLAYERNUM_3:
				m_pSpritePlayerNum[i]->DrawSprite(m_pTexSplitMode[TEX_3P_FONT]);
				break;
			case PLAYERNUM_4:
				m_pSpritePlayerNum[i]->DrawSprite(m_pTexSplitMode[TEX_4P_FONT]);
				break;
			}
		}
		switch (m_editVs)
		{
		case 0:
			m_pSprite2pVs->DrawSprite(m_pTexSplitMode[TEX_2P_VS_FONT]);
			break;
		case 1:
			for (int i = 0; i < MAX_3P_VS; i++)
			{
				switch (i)
				{
				case VS_1P_COM_COM:
					m_pSprite3pVs[i]->DrawSprite(m_pTexSplitMode[TEX_3P_VS_FONT]);
					break;
				case VS_1P_2P_COM:
					m_pSprite3pVs[i]->DrawSprite(m_pTexSplitMode[TEX_3P_VS_FONT]);
					break;
				case VS_1P_2P_3P:
					m_pSprite3pVs[i]->DrawSprite(m_pTexSplitMode[TEX_3P_VS_FONT]);
					break;
				}
			}
			break;
		case 2:
			for (int i = 0; i < MAX_4P_VS; i++)
			{
				switch (i)
				{
				case VS_1P_COM_COM_COM:
					m_pSprite4pVs[i]->DrawSprite(m_pTexSplitMode[TEX_4P_VS_FONT]);
					break;
				case VS_1P_2P_COM_COM:
					m_pSprite4pVs[i]->DrawSprite(m_pTexSplitMode[TEX_4P_VS_FONT]);
					break;
				case VS_1P_2P_3P_COM:
					m_pSprite4pVs[i]->DrawSprite(m_pTexSplitMode[TEX_4P_VS_FONT]);
					break;
				case VS_1P_2P_3P_4P:
					m_pSprite4pVs[i]->DrawSprite(m_pTexSplitMode[TEX_4P_VS_FONT]);
					break;
				}
			}
			break;
		}

		for (int i = 0; i < MAX_TEAM; i++)
		{
			switch (i)
			{
			case 0:
				m_pSpriteEnemyNum[i]->DrawSprite(m_pTexSplitMode[TEX_ENEMYTEAM]);
				break;
			case 1:
				m_pSpriteEnemyNum[i]->DrawSprite(m_pTexSplitMode[TEX_1TEAM]);
				break;
			case 2:
				m_pSpriteEnemyNum[i]->DrawSprite(m_pTexSplitMode[TEX_2TEAM]);
				break;
			case 3:
				m_pSpriteEnemyNum[i]->DrawSprite(m_pTexSplitMode[TEX_3TEAM]);
				break;
			}
		}
		break;
		//モード別分岐
	case MODE_SET_GAMEPARAM:
		//選択枠の描画
		m_pSpritePick->DrawSprite(m_pTexSplitMode[TEX_PICK]);

		//ステージ別描画
		for (int i = 0; i < MAX_STAGE_TYPE; i++)
		{
			switch (i)
			{//教室描画
			case 0:
				m_pSpriteStage[i]->DrawSprite(m_pTexSplitMode[TEX_FIELD_ROOM]);
				break;
				//路地描画
			case 1:
				m_pSpriteStage[i]->DrawSprite(m_pTexSplitMode[TEX_FIELD_STREET]);
				break;
			}
		}

		//キャラクタ・バケツ描画
		for (int j = 0; j < MAX_TEAMMEMBER; j++)
		{//キャラクタ別描画

		 //メンバ別キャラクタの描画
			if (m_editChara == j)
			{//編集中のメンバのキャラクタ描画
				switch (m_charaType)
				{
				case 0:
					m_pSpriteCharacter[j][m_charaType]->DrawSprite(m_pTexSplitMode[TEX_CHARA_LOUCUST]);
					break;
				case 1:
					m_pSpriteCharacter[j][m_charaType]->DrawSprite(m_pTexSplitMode[TEX_CHARA_FIREFLY]);
					break;
				case 2:
					m_pSpriteCharacter[j][m_charaType]->DrawSprite(m_pTexSplitMode[TEX_CHARA_BETTLE]);
					break;
				}
			}
			//未選択のメンバ
			else
			{//メンバのキャラクタ描画
				switch (m_gameParam.characterType[j + (m_playerNo - 1) * MAX_TEAMMEMBER])
				{//メンバ別に(選択された or 未選択)の描画
				case 0:
					m_pSpriteCharacter[j][0]->DrawSprite(m_pTexSplitMode[TEX_CHARA_LOUCUST]);
					break;
				case 1:
					m_pSpriteCharacter[j][1]->DrawSprite(m_pTexSplitMode[TEX_CHARA_FIREFLY]);
					break;
				case 2:
					m_pSpriteCharacter[j][2]->DrawSprite(m_pTexSplitMode[TEX_CHARA_BETTLE]);
					break;
				}
			}
			//メンバ別のバケツ描画
			if (m_editBucket == j)
			{
				switch (m_bucketType)
				{//バケツタイプ滅の描画
				case 0:
					m_pSpriteBucket[j][0]->DrawSprite(m_pTexSplitMode[TEX_BUCKET_VERTICAL]);
					break;
				case 1:
					m_pSpriteBucket[j][1]->DrawSprite(m_pTexSplitMode[TEX_BUCKET_HORIZONTAL]);
					break;
				}
			}
			else
			{//メンバのキャラクタ描画
				switch (m_gameParam.bucketType[j + (m_playerNo - 1) * MAX_TEAMMEMBER])
				{//メンバ別に(選択された or 未選択)の描画
				case 0:
					m_pSpriteBucket[j][m_gameParam.bucketType[j]]->DrawSprite(m_pTexSplitMode[TEX_BUCKET_VERTICAL]);
					break;
				case 1:
					m_pSpriteBucket[j][m_gameParam.bucketType[j]]->DrawSprite(m_pTexSplitMode[TEX_BUCKET_HORIZONTAL]);
					break;
				}
			}
		}
		for (int i = 0; i < MAX_TEAM; i++)
		{
			switch (i)
			{
			case 0:
				m_pSpritePlayerNo[i]->DrawSprite(m_pTexSplitMode[TEX_PLAYER1]);
				break;
			case 1:
				m_pSpritePlayerNo[i]->DrawSprite(m_pTexSplitMode[TEX_PLAYER2]);
				break;
			case 2:
				m_pSpritePlayerNo[i]->DrawSprite(m_pTexSplitMode[TEX_PLAYER3]);
				break;
			case 3:
				m_pSpritePlayerNo[i]->DrawSprite(m_pTexSplitMode[TEX_PLAYER4]);
				break;
			}
		}
		m_pSpriteNext->DrawSprite(m_pTexSplitMode[TEX_NEXT]);
		m_pSpriteRelease->DrawSprite(m_pTexSplitMode[TEX_RELEASE]);
	}
}
//==============================================
//		モードの設定
//==============================================
void CSelect::SelectMode()
{
	//-----コントローラが足りないときの色設定----
		// マルチを薄く
	if (m_selectplayer == 0) {
		if (GetPadNum() < 2) {
			m_pSpriteModeTex[TYPE_MULTI]->m_color = D3DCOLOR_ARGB(100, 255, 255, 255);
		}
		else
			m_pSpriteModeTex[TYPE_MULTI]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
	}


	// VSのとこを薄く
	if (m_selectplayer == 1) {
		m_pSprite2pVs->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
		for (int i = 0; i < MAX_3P_VS; i++) {
			m_pSprite3pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
		}
		for (int i = 0; i < MAX_4P_VS; i++) {
			m_pSprite4pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
		}
	}
	if (m_selectplayer == 2) {
		m_pSprite2pVs->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		for (int i = 0; i < MAX_3P_VS; i++) {
			m_pSprite3pVs[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
		for (int i = 0; i < MAX_4P_VS; i++) {
			m_pSprite4pVs[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}

		switch (GetPadNum())
		{
		case 1:
			m_pSprite4pVs[1]->m_color = D3DCOLOR_ARGB(100, 255, 255, 255);
			m_pSprite3pVs[1]->m_color = D3DCOLOR_ARGB(100, 255, 255, 255);
		case 2:
			m_pSprite4pVs[2]->m_color = D3DCOLOR_ARGB(100, 255, 255, 255);
			m_pSprite3pVs[2]->m_color = D3DCOLOR_ARGB(100, 255, 255, 255);
		case 3:
			m_pSprite4pVs[3]->m_color = D3DCOLOR_ARGB(100, 255, 255, 255);
		default:
			break;
		}
	}


	if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_LSTICK_DOWN) || DIGetKeyTrigger(DIK_DOWNARROW))
	{
		switch (m_selectplayer)
		{
		case 0:
			m_ModeType++;
			m_ModeType %= MAX_MODE_TYPE;
			break;
		case 1:
			m_selectnow++;
			m_selectnow %= MAX_PLAYERNUM;
			break;
		case 2:

			m_selectVsNow++;
			m_selectVsNow %= m_vsDivideNum;
			break;
		case 3:
			int old = m_selectEnemyNum;

			m_selectEnemyNum++;
			m_selectEnemyNum %= MAX_TEAM;
			if (m_selectEnemyNum == 0)
			{
				switch (old)
				{
				case 1:
					m_selectEnemyNum = 3;
					break;
				case 3:
					m_selectEnemyNum = 1;
					break;
				}
			}
			break;
		}
	}

	else if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_LSTICK_UP) || DIGetKeyTrigger(DIK_UPARROW))
	{
		switch (m_selectplayer)
		{
		case 0:
			m_ModeType--;
			m_ModeType = (m_ModeType + MAX_MODE_TYPE) % MAX_MODE_TYPE;
			break;
		case 1:
			m_selectnow--;
			m_selectnow = (m_selectnow + MAX_PLAYERNUM) % MAX_PLAYERNUM;
			break;
		case 2:
			m_selectVsNow--;
			m_selectVsNow = (m_selectVsNow + m_vsDivideNum) % m_vsDivideNum;
			break;
		case 3:

			int old = m_selectEnemyNum;
			m_selectEnemyNum--;
			m_selectEnemyNum = ((m_selectEnemyNum + MAX_TEAM) % MAX_TEAM);

			if (m_selectEnemyNum == 0)
			{
				switch (old)
				{
				case 1:
					m_selectEnemyNum = 3;
					break;
				case 3:
					m_selectEnemyNum = 1;
					break;
				}
			}
			break;

		}
	}

	switch (m_ModeType)
	{
	case TYPE_SINGLE:
		m_pSpriteModeTex[TYPE_MULTI]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
		m_pSpriteModeTex[TYPE_SINGLE]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
		m_pSpriteModeTex[TYPE_SINGLE]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
		m_gameParam.teamNum = 1;
		m_gameParam.playerNum = 1;

		if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN))
		{
			if (m_selectplayer == 3)
			{
				m_setParamOK = true;
			}
			m_selectplayer = 3;
			for (int i = 0; i < MAX_MODE_TYPE; i++)
				m_pSpriteModeTex[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
			for (int i = 0; i < MAX_TEAM; i++)
			{
				m_pSpriteEnemyNum[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
			}
		}
		else if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_B) || DIGetKeyTrigger(DIK_BACK))
		{
			m_selectplayer = 0;
			for (int i = 0; i < MAX_MODE_TYPE; i++)
				m_pSpriteModeTex[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
			for (int i = 0; i < MAX_TEAM; i++)
			{
				m_pSpriteEnemyNum[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
			}
		}
		switch (m_selectEnemyNum)
		{
		case 1:
			m_pSpriteEnemyNum[2]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
			m_pSpriteEnemyNum[3]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
			m_pSpriteEnemyNum[1]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
			m_pSpriteEnemyNum[1]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
			m_gameParam.playerNum = 1;
			m_gameParam.teamNum = 2;
			break;
		case 2:
			m_pSpriteEnemyNum[1]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
			m_pSpriteEnemyNum[3]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
			m_pSpriteEnemyNum[2]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
			m_pSpriteEnemyNum[2]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
			m_gameParam.playerNum = 1;
			m_gameParam.teamNum = 3;
			break;
		case 3:
			m_pSpriteEnemyNum[1]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
			m_pSpriteEnemyNum[2]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
			m_pSpriteEnemyNum[3]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
			m_pSpriteEnemyNum[3]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
			m_gameParam.playerNum = 1;
			m_gameParam.teamNum = 4;
			break;
		}
		break;

	case TYPE_MULTI:
		switch (m_selectplayer)
		{
		case 0:
			m_pSpriteModeTex[TYPE_SINGLE]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
			m_pSpriteModeTex[TYPE_MULTI]->m_scale.x += sin(m_scaleChange*D3DX_PI)*0.01f;
			m_pSpriteModeTex[TYPE_MULTI]->m_scale.y += sin(m_scaleChange*D3DX_PI)*0.01f;
			m_setParamOK = false;

			if (GetPadNum() < 2)	break;		// 修正
			if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN))
			{
				m_selectplayer = 1;
				for (int i = 0; i < MAX_MODE_TYPE; i++)
					m_pSpriteModeTex[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
				for (int i = 0; i < MAX_PLAYERNUM; i++)
				{
					m_pSpritePlayerNum[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
				}
			}
			break;
		case 1:
			switch (m_selectnow)
			{
			case PLAYERNUM_2:
				m_pSpritePlayerNum[PLAYERNUM_3]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
				m_pSpritePlayerNum[PLAYERNUM_4]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
				m_pSpritePlayerNum[PLAYERNUM_2]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
				m_pSpritePlayerNum[PLAYERNUM_2]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
				m_editVs = PLAYERNUM_2;
				break;
			case PLAYERNUM_3:
				m_pSpritePlayerNum[PLAYERNUM_2]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
				m_pSpritePlayerNum[PLAYERNUM_4]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
				m_pSpritePlayerNum[PLAYERNUM_3]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
				m_pSpritePlayerNum[PLAYERNUM_3]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
				m_editVs = PLAYERNUM_3;
				break;
			case PLAYERNUM_4:
				m_pSpritePlayerNum[PLAYERNUM_2]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
				m_pSpritePlayerNum[PLAYERNUM_3]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
				m_pSpritePlayerNum[PLAYERNUM_4]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
				m_pSpritePlayerNum[PLAYERNUM_4]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
				m_editVs = PLAYERNUM_4;
				break;
			}
			if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN))
			{
				m_selectplayer = 2;

				switch (m_editVs)
				{
				case PLAYERNUM_2:
					m_vsDivideNum = 1;
					m_pSprite2pVs->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
					for (int i = 0; i < MAX_3P_VS; i++)
						m_pSprite3pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					for (int i = 0; i < MAX_4P_VS; i++)
						m_pSprite4pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					break;
				case PLAYERNUM_3:
					m_vsDivideNum = MAX_3P_VS;
					m_pSprite2pVs->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					for (int i = 0; i < MAX_3P_VS; i++)
						m_pSprite3pVs[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
					for (int i = 0; i < MAX_4P_VS; i++)
						m_pSprite4pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					break;
				case PLAYERNUM_4:
					m_vsDivideNum = MAX_4P_VS;
					m_pSprite2pVs->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					for (int i = 0; i < MAX_3P_VS; i++)
						m_pSprite3pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					for (int i = 0; i < MAX_4P_VS; i++)
						m_pSprite4pVs[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
					break;
				}

			}
			else if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_B) || DIGetKeyTrigger(DIK_BACK))
			{
				for (int i = 0; i < MAX_PLAYERNUM; i++)
				{
					m_pSpritePlayerNum[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
				}

				for (int i = 0; i < MAX_MODE_TYPE; i++)
					m_pSpriteModeTex[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
				m_selectplayer = 0;
			}
			break;
		case 2:
			switch (m_editVs)
			{
			case PLAYERNUM_2:
				m_pSprite2pVs->m_scale = D3DXVECTOR2(1.0f, 1.0f);
				m_pSprite2pVs->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
				m_pSprite2pVs->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
				m_gameParam.teamNum = 2;
				m_gameParam.playerNum = 2;

				break;
			case PLAYERNUM_3:
				m_gameParam.teamNum = 3;
				switch (m_selectVsNow)
				{
				case 0:
					m_pSprite3pVs[1]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite3pVs[2]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite3pVs[m_selectVsNow]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_pSprite3pVs[m_selectVsNow]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;

					m_gameParam.playerNum = 1;
					break;
				case 1:
					m_pSprite3pVs[0]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite3pVs[2]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite3pVs[m_selectVsNow]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_pSprite3pVs[m_selectVsNow]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_gameParam.playerNum = 2;
					break;
				case 2:
					m_pSprite3pVs[0]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite3pVs[1]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite3pVs[m_selectVsNow]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_pSprite3pVs[m_selectVsNow]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_gameParam.playerNum = 3;
					break;
				}
				break;
			case PLAYERNUM_4:
				m_gameParam.teamNum = 4;
				switch (m_selectVsNow)
				{
				case 0:
					m_pSprite4pVs[1]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[2]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[3]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[m_selectVsNow]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_pSprite4pVs[m_selectVsNow]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_gameParam.playerNum = 1;
					break;
				case 1:
					m_pSprite4pVs[0]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[1]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[2]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[m_selectVsNow]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_pSprite4pVs[m_selectVsNow]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_gameParam.playerNum = 2;
					break;
				case 2:
					m_pSprite4pVs[0]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[1]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[3]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[m_selectVsNow]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_pSprite4pVs[m_selectVsNow]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_gameParam.playerNum = 3;
					break;
				case 3:
					m_pSprite4pVs[0]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[1]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[3]->m_scale = D3DXVECTOR2(1.0f, 1.0f);
					m_pSprite4pVs[m_selectVsNow]->m_scale.x += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_pSprite4pVs[m_selectVsNow]->m_scale.y += sin(m_scaleChange * D3DX_PI)*0.01f;
					m_gameParam.playerNum = 4;
					break;
				}
				break;
			}
			if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN))
			{
				if (m_gameParam.playerNum > GetPadNum()) break;
				m_setParamOK = true;
			}
			else if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_B) || DIGetKeyTrigger(DIK_BACK))
			{
				switch (m_editVs)
				{
				case PLAYERNUM_2:
					m_pSprite2pVs->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					break;
				case PLAYERNUM_3:
					for (int i = 0; i < MAX_3P_VS; i++)
						m_pSprite3pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					break;
				case PLAYERNUM_4:
					for (int i = 0; i < MAX_4P_VS; i++)
						m_pSprite4pVs[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					break;
				}
				m_selectplayer = 1;
			}
			break;
		}
		break;
	}

	m_scaleChange += 0.1f;
}

//==============================================
//		ゲーム情報の設定
//==============================================
void CSelect::SetGamePrm()
{
	m_sumChara = m_gameParam.teamNum;
	m_playerNum = m_gameParam.playerNum;

	//プレイヤー番号
	for (int i = 0; i < MAX_TEAM; i++)
	{
		if (i == (m_playerNo - 1))
		{
			m_pSpritePlayerNo[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
		else
		{
			m_pSpritePlayerNo[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
		}
	}

	//枠の移動
	if (m_setStateFlag == 0)
	{
		if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_DOWN) || DIGetKeyTrigger(DIK_DOWN))
		{
			m_range++;

			m_range %= MAX_SELECT_PARAM;
		}
		else if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_UP) || DIGetKeyTrigger(DIK_UP))
		{
			m_range--;
			if (m_range < 0)
			{
				m_range *= -(MAX_SELECT_PARAM - 1);
			}
		}
		if (m_playerNo > 1)
		{
			for (int i = 0; i < MAX_STAGE_TYPE; i++)
				m_pSpriteStage[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);

			if (m_range == SELECT_STAGE)
			{
				switch (m_rangeOld)
				{
				case SELECT_TEAMMEMBER:
					m_range = SELECT_BACK;
					break;
				case SELECT_BACK:
					m_range = SELECT_TEAMMEMBER;
					break;
				}
			}
		}
	}


	//モード別選択
	switch (m_range)
	{//設定する区分の分岐
	 //＝＝＝＝＝＝＝＝＝ステージ選択＝＝＝＝＝＝＝＝＝
	case SELECT_STAGE:
		StagePrm();
		break;
		//＝＝＝＝＝＝＝＝＝キャラクタ選択＝＝＝＝＝＝＝＝＝
	case SELECT_TEAMMEMBER:
		CharacterPrm();
		break;
		//＝＝＝＝＝＝＝＝＝バケツ選択＝＝＝＝＝＝＝＝＝
	case SELECT_USE_BUCKET:
		BucketPrm();
		break;
	case SELECT_NEXT:
		if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN))
			if (m_playerNum == m_playerNo)
			{
				m_modeNo = MODE_SET_GAME;
			}
			else
			{
				m_playerNo++;

				m_charaType = 0;
				m_setStateFlag = 0;
				m_editChara = -1;

				m_pickNo = 0;
				m_bucketType = 0;
				m_editBucket = -1;

				for (int i = 0; i < MAX_TEAMMEMBER; i++)
				{
					m_setCharaFlag[i] = -1;
					m_setBucketFlag[i] = -1;
					m_charaNo[i + m_playerNo * MAX_TEAMMEMBER] = -1;
					m_bucketNo[i + (m_playerNo - 1)*MAX_TEAMMEMBER] = -1;

					for (int j = 0; j < MAX_BUCKET_TYPE; j++)
						m_pSpriteBucket[i][j]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
					for (int j = 0; j < MAX_CHARACTER_TYPE; j++)
						m_pSpriteCharacter[i][j]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);

				}

				m_pSpritePick->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);

			}
		if (m_range != m_rangeOld)
		{
			m_pickPrm.pos = m_nextPrm.pos;
			m_pickPrm.size = m_nextPrm.size + D3DXVECTOR2(10.0f, 10.0f);
			m_rangeOld = m_range;
		}
		m_pSpritePick->m_pos = m_pickPrm.pos;
		m_pSpritePick->m_width = m_pickPrm.size.x;
		m_pSpritePick->m_height = m_pickPrm.size.y;
		break;
	case SELECT_BACK:
		if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN))
		{
			switch (m_playerNo)
			{
			case 1:

				m_modeNo = MODE_SELECT;
				m_gameParam.teamNum = 0;
				m_gameParam.playerNum = 0;
				m_pushFlag = false;
				m_setParamOK = false;
				break;
			case 2:
			case 3:
			case 4:
				m_playerNo--;
				m_charaType = 0;

				for (int i = 0; i < MAX_TEAMMEMBER; i++)
				{
					m_charaNo[i + (m_playerNo - 1) * MAX_TEAMMEMBER] = m_gameParam.characterType[i + (m_playerNo - 1) * 3];
					m_bucketNo[i + (m_playerNo - 1)*MAX_TEAMMEMBER] = m_gameParam.bucketType[i + (m_playerNo - 1) * 3];



					for (int j = 0; j < MAX_BUCKET_TYPE; j++)
						m_pSpriteBucket[i][j]->m_color = D3DCOLOR_ARGB(255, 100, 100, 100);
					for (int j = 0; j < MAX_CHARACTER_TYPE; j++)
						m_pSpriteCharacter[i][j]->m_color = D3DCOLOR_ARGB(255, 100, 100, 100);
				}
				if (m_playerNo == 1) {
					for (int i = 0; i < MAX_STAGE_TYPE; i++)
						m_pSpriteStage[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
					m_pSpriteStage[m_gameParam.stageType]->m_color = D3DCOLOR_ARGB(255, 100, 100, 100);
				}
				break;
			}
		}

		if (m_range != m_rangeOld)
		{
			m_pickPrm.pos = m_releasePrm.pos;
			m_pickPrm.size = m_releasePrm.size + D3DXVECTOR2(10.0f, 10.0f);
			m_rangeOld = m_range;
		}
		m_pSpritePick->m_pos = m_pickPrm.pos;
		m_pSpritePick->m_width = m_pickPrm.size.x;
		m_pSpritePick->m_height = m_pickPrm.size.y;
		break;
	}
}

//==============================================
//　ステージ設定
//==============================================
void CSelect::StagePrm()
{
	//枠の移動（右）
	if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_RIGHT) || DIGetKeyTrigger(DIK_RIGHTARROW))
	{
		m_pickNo++;
		//最大選択数を超えないように
		m_pickNo %= 2;
	}
	//枠の移動（左）
	else if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_LEFT) || DIGetKeyTrigger(DIK_LEFTARROW))
	{
		m_pickNo--;
		//最小選択数を超えないように
		if (m_pickNo < 0) m_pickNo *= -1;
	}

	//移動先の枠情報取得
	switch (m_pickNo)
	{
		//ステージ１の情報を取得
	case 0:
		m_pickPrm.pos = m_stagePrm[m_pickNo].pos;
		break;
		//ステージ2の情報を取得
	case 1:
		m_pickPrm.pos = m_stagePrm[m_pickNo].pos;
		break;
	}
	//Aボタンが押されたら
	if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN))
	{
		//ゲーム情報にステージ番号を格納
		m_stageNo = m_pickNo;
		for (int i = 0; i < 2; i++)
			if (i == m_pickNo)
				m_pSpriteStage[m_pickNo]->m_color = D3DCOLOR_ARGB(255, 100, 100, 100);
			else
				m_pSpriteStage[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);

	}
	//Bボタンが押されたら
	else if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_B) || DIGetKeyTrigger(DIK_BACK))
	{
		//ゲーム情報にステージ番号を格納
		m_stageNo = -1;
		for (int i = 0; i < MAX_STAGE_TYPE; i++)
			m_pSpriteStage[i]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
	}

	//選択位置情報の設定
	if (m_range != m_rangeOld)
	{
		if (m_pickNo > MAX_STAGE_TYPE - 1)
		{
			m_pickNo = MAX_STAGE_TYPE - 1;
		}
		m_pickPrm.pos = m_stagePrm[m_pickNo].pos;
		m_rangeOld = m_range;
	}
	m_pSpritePick->m_pos = m_pickPrm.pos;
	m_pickPrm.size = m_stagePrm[0].size + D3DXVECTOR2(20.0f, 20.0f);
	m_pSpritePick->m_width = m_pickPrm.size.x;
	m_pSpritePick->m_height = m_pickPrm.size.y;
	if (m_stageNo != -1)
		m_gameParam.stageType = m_stageNo;
	else
		m_gameParam.stageType = 0;
}
//==============================================
//　キャラクタ設定
//==============================================
void CSelect::CharacterPrm()
{
	switch (m_setStateFlag)
	{
		//編集するメンバーの選択
	case 0:
		//移動（右）
		if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_RIGHT) || DIGetKeyTrigger(DIK_RIGHTARROW))
		{
			m_pickNo++;
			m_pickNo %= MAX_TEAMMEMBER;
		}
		//移動（左）
		else if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_LEFT) || DIGetKeyTrigger(DIK_LEFTARROW))
		{
			m_pickNo--;
			if (m_pickNo < 0)
				m_pickNo = (m_pickNo  * -MAX_TEAMMEMBER) - 1;
		}
		break;
		//キャラタイプの選択
	case 1:
		//移動（右）
		if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_RIGHT) || DIGetKeyTrigger(DIK_RIGHTARROW))
		{
			m_charaType++;
			m_charaType %= MAX_CHARACTER_TYPE;
		}
		//移動（左）
		else if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_LEFT) || DIGetKeyTrigger(DIK_LEFTARROW))
		{
			m_charaType--;
			if (m_charaType < 0)
				m_charaType = (m_charaType * -MAX_CHARACTER_TYPE) - 1;
		}
		break;
	}

	//Aボタンが押されたら
	if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN))
	{
		if (m_setStateFlag == 0)
		{//メンバ番号選択
		 //選択した番号の中身があるか？
			if (m_charaNo[m_pickNo + (m_playerNo - 1) * MAX_TEAMMEMBER] >= 0)
			{//あれば何もしない
				m_setStateFlag = 0;
			}
			else
			{//なければ設定する
				m_editChara = m_pickNo;
				m_setStateFlag = 1;
				m_charaType = m_gameParam.characterType[m_editChara + (m_playerNo - 1) * MAX_TEAMMEMBER];
				m_pSpritePick->m_color = D3DCOLOR_ARGB(255, 0, 255, 255);
			}
		}
		else
		{//タイプ番号設定
			m_setStateFlag = 0;
			m_charaNo[m_editChara + (m_playerNo - 1) * MAX_TEAMMEMBER] = m_charaType;
			m_setCharaFlag[m_editChara] = 1;
			m_pSpritePick->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);

		}

		for (int i = 0; i < MAX_TEAMMEMBER; i++)
		{
			if (m_charaNo[i + (m_playerNo - 1) * MAX_TEAMMEMBER] >= 0)
			{
				for (int j = 0; j < MAX_CHARACTER_TYPE; j++)
					m_pSpriteCharacter[i][j]->m_color = D3DCOLOR_ARGB(255, 100, 100, 100);
			}
		}

	}
	//Bボタンが押されたら
	else if (GetPadButton((m_playerNo - 1), PAD_FLAG_TRIGGER, PAD_B) || DIGetKeyTrigger(DIK_BACK))
	{
		if (m_charaNo[m_pickNo + (m_playerNo - 1) * MAX_TEAMMEMBER] >= 0)
		{
			m_charaNo[m_pickNo + (m_playerNo - 1) * MAX_TEAMMEMBER] = -1;
			m_setCharaFlag[m_pickNo] = -1;
			for (int j = 0; j < MAX_CHARACTER_TYPE; j++)
			{
				m_pSpriteCharacter[m_pickNo][j]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
			}
		}
	}
	//設定キャラクタの選択
	switch (m_pickNo)
	{
	case 0:
		m_pickPrm.pos = m_characterPrm[m_pickNo][0].pos;
		break;
	case 1:
		m_pickPrm.pos = m_characterPrm[m_pickNo][1].pos;
		break;
	case 2:
		m_pickPrm.pos = m_characterPrm[m_pickNo][2].pos;
		break;
	}

	//各情報の設定
	for (int i = 0; i < MAX_CHARACTER_TYPE; i++)
	{
		if (m_charaNo[i + (m_playerNo - 1) * MAX_TEAMMEMBER] < 0)continue;
		m_gameParam.characterType[i + (m_playerNo - 1) * MAX_TEAMMEMBER] = m_charaNo[i + (m_playerNo - 1) * MAX_TEAMMEMBER];
	}

	if (m_range != m_rangeOld)
	{
		if (m_pickNo >= MAX_STAGE_TYPE - 1)
		{
			m_pickNo = MAX_TEAMMEMBER - 1;
		}
		m_pickPrm.pos = m_characterPrm[m_pickNo][0].pos;
		m_rangeOld = m_range;
	}
	m_pSpritePick->m_pos = m_pickPrm.pos;
	m_pickPrm.size = m_characterPrm[0][0].size + D3DXVECTOR2(20.0f, 20.0f);
	m_pSpritePick->m_width = m_pickPrm.size.x;
	m_pSpritePick->m_height = m_pickPrm.size.y;
}

//==============================================
//　バケツ設定
//==============================================
void CSelect::BucketPrm()
{
	switch (m_setStateFlag)
	{
		//編集するメンバーの選択
	case 0:
		//移動（右）
		if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_RIGHT) || DIGetKeyTrigger(DIK_RIGHTARROW))
		{
			m_pickNo++;
			m_pickNo %= MAX_TEAMMEMBER;
		}
		//移動（左）
		else if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_LEFT) || DIGetKeyTrigger(DIK_LEFTARROW))
		{
			m_pickNo--;
			if (m_pickNo < 0)
				m_pickNo = (m_pickNo  * -MAX_TEAMMEMBER) - 1;
		}
		break;

		//キャラタイプの選択
	case 1:

		//移動（右）
		if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_RIGHT) || DIGetKeyTrigger(DIK_RIGHTARROW))
		{
			m_bucketType++;
			m_bucketType %= MAX_BUCKET_TYPE;
		}
		//移動（左）
		else if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_LSTICK_LEFT) || DIGetKeyTrigger(DIK_LEFTARROW))
		{
			m_bucketType--;
			if (m_bucketType < 0)
				m_bucketType = (m_bucketType * -MAX_BUCKET_TYPE) - 1;
		}
		break;
	}

	//Aボタンが押されたら
	if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN))
	{
		//中身の設定するか決める
		if (m_setStateFlag == 0)
		{//バケツ番号に中身があるか?
			if (m_bucketNo[m_pickNo + (m_playerNo - 1)*MAX_TEAMMEMBER] >= 0)
			{//あれば何もしない
				m_setStateFlag = 0;
			}
			else
			{//なければ中身の設定に移動
				m_editBucket = m_pickNo;
				m_setStateFlag = 1;

				m_bucketType = m_gameParam.bucketType[m_editBucket + (m_playerNo - 1) * MAX_TEAMMEMBER];
				m_pSpritePick->m_color = D3DCOLOR_ARGB(255, 0, 255, 255);
			}
		}
		//中身の設定
		else
		{
			m_setStateFlag = 0;
			m_bucketNo[m_editBucket + (m_playerNo - 1)*MAX_TEAMMEMBER] = m_bucketType;
			m_setBucketFlag[m_editBucket] = 1;
			m_pSpritePick->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		}

		for (int i = 0; i < MAX_TEAMMEMBER; i++)
		{
			if (m_bucketNo[i + (m_playerNo - 1)*MAX_TEAMMEMBER] >= 0)
			{
				for (int j = 0; j < MAX_BUCKET_TYPE; j++)
					m_pSpriteBucket[i][j]->m_color = D3DCOLOR_ARGB(255, 100, 100, 100);
			}
		}
	}
	//Bボタンが押されたら
	else if (GetPadButton(m_playerNo - 1, PAD_FLAG_TRIGGER, PAD_B) || DIGetKeyTrigger(DIK_BACK))
	{
		if (m_bucketNo[m_pickNo + (m_playerNo - 1)*MAX_TEAMMEMBER] >= 0)
		{
			m_bucketNo[m_pickNo + (m_playerNo - 1)*MAX_TEAMMEMBER] = -1;
			m_setBucketFlag[m_pickNo] = -1;
			for (int j = 0; j < MAX_BUCKET_TYPE; j++)
			{
				m_pSpriteBucket[m_pickNo][j]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
			}
		}
	}

	//設定キャラクタの選択
	switch (m_pickNo)
	{
	case 0:
		m_pickPrm.pos = m_bucketPrm[m_pickNo][0].pos;
		break;
	case 1:
		m_pickPrm.pos = m_bucketPrm[m_pickNo][1].pos;
		break;
	case 2:
		m_pickPrm.pos = m_bucketPrm[m_pickNo][1].pos;		// ?
		break;
	}

	for (int i = 0; i < MAX_TEAMMEMBER; i++)
	{

		if (m_bucketNo[i + (m_playerNo - 1) * MAX_TEAMMEMBER] < 0)continue;
		m_gameParam.bucketType[i + (m_playerNo - 1) * MAX_TEAMMEMBER] = m_bucketNo[i + (m_playerNo - 1) * MAX_TEAMMEMBER];
	}
	if (m_range != m_rangeOld)
	{
		if (m_pickNo >= MAX_STAGE_TYPE - 1)
		{
			m_pickNo = MAX_TEAMMEMBER - 1;
		}
		m_pickPrm.pos = m_bucketPrm[m_pickNo][0].pos;
		m_rangeOld = m_range;
	}
	m_pSpritePick->m_pos = m_pickPrm.pos;
	m_pickPrm.size = m_bucketPrm[0][0].size + D3DXVECTOR2(20.0f, 20.0f);
	m_pSpritePick->m_width = m_pickPrm.size.x;
	m_pSpritePick->m_height = m_pickPrm.size.y;
}

//==============================================
//		テクスチャの読み込み
//==============================================
void CSelect::LoadTexture()
{
	for (int i = 0; i < MAX_TEX; i++)
	{
		// バックバッファのアドレス取得
		m_pDevice->GetRenderTarget(0, &m_pBackBuffer);

		// テクスチャ作成
		m_pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexSplitMode[i], NULL);
		// テクスチャからサーフェイスを作成
		m_pTexSplitMode[i]->GetSurfaceLevel(0, &m_pSurfaceSplitSelect);
		switch (i)
		{
		case TEX_BACK:
			m_pTexSplitMode[i] = SetTexture(TEX_BACKGROUND_PATH);
			break;
		case TEX_SINGLE_FONT:
			m_pTexSplitMode[i] = SetTexture(TEX_SINGLE_PATH);
			break;
		case TEX_MULTI_FONT:
			m_pTexSplitMode[i] = SetTexture(TEX_MULTI_PATH);
			break;
		case TEX_2P_FONT:
			m_pTexSplitMode[i] = SetTexture(TEX_2P_PATH);
			break;
		case TEX_3P_FONT:
			m_pTexSplitMode[i] = SetTexture(TEX_3P_PATH);
			break;
		case TEX_4P_FONT:
			m_pTexSplitMode[i] = SetTexture(TEX_4P_PATH);
			break;
		case TEX_2P_VS_FONT:
			m_pTexSplitMode[i] = SetTexture(TEX_VS_2P_PATH);
			break;
		case TEX_3P_VS_FONT:
			m_pTexSplitMode[i] = SetTexture(TEX_VS_3P_PATH);
			break;
		case TEX_4P_VS_FONT:
			m_pTexSplitMode[i] = SetTexture(TEX_VS_4P_PATH);
			break;
		case TEX_PICK:
			m_pTexSplitMode[i] = SetTexture(TEX_PICK_PATH);
			break;
		case TEX_FIELD_ROOM:
			m_pTexSplitMode[i] = SetTexture(TEX_STAGE_ROOM_PATH);
			break;
		case TEX_FIELD_STREET:
			m_pTexSplitMode[i] = SetTexture(TEX_STAGE_ROAD_PATH);
			break;
		case TEX_CHARA_LOUCUST:
			m_pTexSplitMode[i] = SetTexture(TEX_CHARACTER_LOCUST_PATH);
			break;
		case TEX_CHARA_FIREFLY:
			m_pTexSplitMode[i] = SetTexture(TEX_CHARACTER_FIREFLY_PATH);
			break;
		case TEX_CHARA_BETTLE:
			m_pTexSplitMode[i] = SetTexture(TEX_CHARACTER_BETTLE_PATH);
			break;
		case TEX_BUCKET_VERTICAL:
			m_pTexSplitMode[i] = SetTexture(TEX_BUCKET_VERTICAL_PATH);
			break;
		case TEX_BUCKET_HORIZONTAL:
			m_pTexSplitMode[i] = SetTexture(TEX_BUCKET_HORIZONTAL_PATH);
			break;
		case TEX_NEXT:
			m_pTexSplitMode[i] = SetTexture(TEX_NEXT_PATH);
			break;
		case TEX_ENEMYTEAM:
			m_pTexSplitMode[i] = SetTexture(TEX_ENEMYTEAM_PATH);
			break;
		case TEX_1TEAM:
			m_pTexSplitMode[i] = SetTexture(TEX_1TEAM_PATH);
			break;
		case TEX_2TEAM:
			m_pTexSplitMode[i] = SetTexture(TEX_2TEAM_PATH);
			break;
		case TEX_3TEAM:
			m_pTexSplitMode[i] = SetTexture(TEX_3TEAM_PATH);
			break;
		case TEX_PLAYER1:
			m_pTexSplitMode[i] = SetTexture(TEX_PLAYER1_PATH);
			break;
		case TEX_PLAYER2:
			m_pTexSplitMode[i] = SetTexture(TEX_PLAYER2_PATH);
			break;
		case TEX_PLAYER3:
			m_pTexSplitMode[i] = SetTexture(TEX_PLAYER3_PATH);
			break;
		case TEX_PLAYER4:
			m_pTexSplitMode[i] = SetTexture(TEX_PLAYER4_PATH);
			break;
		case TEX_RELEASE:
			m_pTexSplitMode[i] = SetTexture(TEX_RELEASE_PATH);
			break;
		}
		m_pBackBuffer = NULL;
	}
}