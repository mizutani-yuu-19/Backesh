///////////////////////////////////////////////////
//
// ゲームシーン
//
//////////////////////////////////////////////////
//----------------インクルード--------------
#include "game.h"
#include "character.h"
#include "water_puddle.h"
#include "shadow.h"
#include "light.h"
#include "camera.h"
#include "water_atk.h"
#include "stage.h"
#include "UI.h"
#include "fade.h"
#include "skydome.h"
#include "sound.h"
#include "effect.h"
#include "game_pad.h"
#include "input.h"

//--------------定数マクロ-----------
ST_GAMEPARAM CGame::m_gameParam;
int		CGame::m_gameTime;
int		CGame::m_step = STEP_STAGEVIEW;

//---------------メンバ関数---------------
// コンストラクタ
CGame::CGame()
{
//	ゲーム用情報の初期化
	//m_gameParam.stageType = 1;		// ステージの種類
	//m_gameParam.playerNum = 4;		// プレイ人数
	//m_gameParam.teamNum = 4;		// チームの数
	for (int i = m_gameParam.playerNum*MAX_TEAMMEMBER; i < m_gameParam.teamNum*MAX_TEAMMEMBER; i++)
		m_gameParam.bucketType[i] = rand() % 2;
	for (int i = m_gameParam.playerNum*MAX_TEAMMEMBER; i < m_gameParam.teamNum*MAX_TEAMMEMBER; i++)
		m_gameParam.characterType[i] = rand() % 3;

	CCharacter::LoadCharacterModel();
	CWaterPuddle::LoadModel();
	CWaterAtk::CreateBillboard();
	CUI::CreateUI(m_gameParam.playerNum);
	CCamera::CreateStageCamera();
	StartFadeIn();
}

// デストラクタ
CGame::~CGame() {

	Uninit();
}

void CGame::Init() {
	m_gameTime = GAME_TIME;
	//ステージの生成
	CStage::CreateStage(m_gameParam.stageType);
	CSkydome::CreateSkydome(m_gameParam.stageType);

	//キャラクタの生成
	CCharacter::CreateCharacter(m_gameParam.characterType, m_gameParam.playerNum, m_gameParam.teamNum, m_gameParam.bucketType);

	//キャラクタの初期化
	for (int i = 0; i < CCharacter::m_teamNum; i++)
	{
		for (int j = 0; j < MAX_TEAMMEMBER; j++)
		{
			CCharacter::m_character[i][j]->Init();
		}
	}
	CEffect::LoadEffect();

	CUI::AllInit();
	// ライトの初期化処理
	InitLight();

	// 丸影の初期化処理
	//InitShadow();

	PlaySound(SOUND_LABEL_BATTLE, 1.0f);
}

void CGame::Uninit() {
	// キャラクター全消去
	for (int i = 0; i < CCharacter::m_teamNum; i++) {
		for (int j = 0; j < 3; j++) {
			SAFE_DELETE(CCharacter::m_character[i][j]);
		}
	}
	CSkydome::DeleteSkydome();
	CCharacter::ReleaseCharacterModel();
	CWaterPuddle::ReleaseModel();
	CWaterAtk::ReleaseBillboard();
	CCharacter::ReleaseCharacterModel();
	CWaterPuddle::m_waterPuddle.clear();
	CUI::AllUninit();
	// カメラの終了処理
	CCamera::DeleteCamera();
	SAFE_DELETE(CStage::m_stage);
	// ライトの終了処理
	UninitLight();
	CEffect::Uninit();
	// 丸影の終了処理
	//UninitShadow();
}

void CGame::Update() {
	CEffect::Update();
	// キャラクターの更新
	CCharacter::AllUpdate();
	// 水たまりの更新
	CWaterPuddle::UpdateWaterPuddle();
	// カメラの更新処理
	CCamera::UpdateCamera();

	// 光源の更新処理
	UpdateLight();

	// 丸影の更新処理
	UpdateShadow();

	CUI::AllUpdate();
	// ゲームのタイマーを減らす
	if (m_step == STEP_GAME)
		m_gameTime--;
	if (m_gameTime == 0) {
		// ここでリザルトへ移動
		m_step++;
		StartFadeOut();
		m_gameTime--;
	}
	if (m_step == STEP_FINISH&&EndFadeOut())
	{
		StartFadeIn();
		CCamera::DeleteCamera();
		CCamera::CreateResultCamera();
		CCharacter::ResultFunc();
		CUI::AllUninit();
		CUI::CreateResultUI();
		StopSound(SOUND_LABEL_BATTLE);
		PlaySound(SOUND_LABEL_WIN, 1.0f);
	}
	else if (m_step == STEP_FINISH&&EndFadeIn()) {
		if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_A)|| DIGetKeyTrigger(DIK_RETURN)) {
			m_step = STEP_STAGEVIEW;
			StartFadeOut();
		}
	}
	else if (m_step == STEP_STAGEVIEW&&EndFadeOut()) {
		g_pScene->ChangeScene(SCENE_TITLE);
		StopSound(SOUND_LABEL_WIN);
	}
	if (m_step == STEP_CHARACTERVIEW&&EndFadeOut()) {
		m_step++;
	}
}

void CGame::Draw() {

	for (int i = 0; i < CCamera::GetCameraNum(); i++) {

		// カメラの設定処理
		CCamera::SetCamera(i);

		CSkydome::m_skydome->Draw(i);

		CStage::m_stage->Draw(i);

		// 丸影の描画処理
		//DrawShadow();

		CWaterPuddle::DrawWaterPuddle();
		CCharacter::AllDraw(i);
		CEffect::Draw(i);
	}
	CUI::AllDraw();
}

void CGame::SetGameParam(ST_GAMEPARAM param)
{
	m_gameParam = param;
}

ST_GAMEPARAM CGame::GetGameParam() {
	return m_gameParam;
}

int CGame::GetTime() {
	return m_gameTime / 60;
}