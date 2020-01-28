///////////////////////////////////////////////////
//
//			キャラクター
//
//////////////////////////////////////////////////
//----------------インクルード--------------
#include "character.h"
#include "player.h"
#include "AI.h"
#include "stage.h"
#include "camera.h"
#include "collision.h"
#include "input.h"
#include "debugproc.h"
#include "debug.h"
#include "game_pad.h"
#include "sound.h"
#include "model.h" 
#include "fade.h"
//-------------定数マクロ--------------
#define RESPAWNTIME	(120)	// リスポンまでの時間
#define PICKUP_TIME	(30)	// 水をすくう最大時間
#define PLAYER_MOVE_SPEED (4)
#define SKILL_ACCUMULATE	(1.0f/3600.0f)			// 一分間でスキルゲージマックス
//---------静的メンバ-----------
CCharacter*				CCharacter::m_character[MAX_TEAM][MAX_TEAMMEMBER];
int						CCharacter::m_teamNum;											//チーム数

int						CCharacter::m_killAllCnt[MAX_TEAM];								// チームのトータルキルカウント
int						CCharacter::m_deathAllCnt[MAX_TEAM];							// チームのトータルデスカウント

CCharacter*				CCharacter::changeFrom = NULL;									// チェンジ元
CCharacter*				CCharacter::changeTo = NULL;									// チェンジ先
										// メッシュ
int						CCharacter::m_contentPos[MAX_TEAM][MAX_TEAMMEMBER] = { {0,1,2},{0,1,2},{0,1,2},{0,1,2} };
void SetCollisionWall(D3DXVECTOR3*, D3DXVECTOR3*);

int CCharacter::m_winner;

MODEL					CCharacter::m_bucketModel[2] = { NULL,NULL };
LPDIRECT3DTEXTURE9		CCharacter::m_pD3DTextureLockOn;
//---------------メンバ関数---------------

//************ static関数 *******************
//キャラクター作成
void CCharacter::CreateCharacter(int *type, int playerNum, int teamNum, int *bucketType)
{
	for (int i = 0; i < MAX_TEAM; i++) {
		m_killAllCnt[i] = 0;
		m_deathAllCnt[i] = 0;
		for (int j = 0; j < MAX_TEAMMEMBER; j++) {
			m_character[i][j] = NULL;
		}
	}
	m_winner = -1;
	m_teamNum = teamNum;
	for (int i = 0; i < teamNum * MAX_TEAMMEMBER; i++) {
		if (i%MAX_TEAMMEMBER == 0) {
			if (playerNum > 0) {
				m_character[0][i] = new CPlayer(type[i], (int)(i * (1.0f / MAX_TEAMMEMBER + 0.01f)), bucketType[i]);
				m_character[0][i]->m_playerFlag = true;
				playerNum--;
			}
			else {
				m_character[0][i] = new CAi(type[i], (int)(i * (1.0f / MAX_TEAMMEMBER + 0.01f)), bucketType[i]);
				m_character[0][i]->m_playerFlag = false;
			}
		}
		else {
			m_character[0][i] = new CAi(type[i], (int)(i * (1.0f / MAX_TEAMMEMBER + 0.01f)), bucketType[i]);
			m_character[0][i]->m_playerFlag = false;
		}
	}
}
//全更新
void CCharacter::AllUpdate() {
	for (int i = 0; i < m_teamNum*MAX_TEAMMEMBER; i++) {
		m_character[0][i]->Update();
		if (i % MAX_TEAMMEMBER == 2) {
			// チェンジ
			if (changeFrom) {
				ChangePlayer();
				changeFrom = NULL;
				changeTo = NULL;
			}
		}
	}
}
//全描画
void CCharacter::AllDraw(int camNo) {
	for (int i = 0; i < m_teamNum; i++) {
		if (CGame::GetStep() == STEP_FINISH&&EndFadeOut()) {
			if (GetWinner() != i)	continue;
		}
		for (int j = 0; j < MAX_TEAMMEMBER; j++)
			m_character[i][j]->Draw(camNo);
	}
}
//プレイヤーの位置を返す
//多分カメラで使うだけ
D3DXVECTOR3 CCharacter::GetPlayerPos(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_afterPos;
		}
	}
}
//プレイヤーの移動量を返す
D3DXVECTOR3 CCharacter::GetPlayerMove(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_move;
		}
	}
}
//プレイヤーの回転方向を返す
float CCharacter::GetPlayerRot(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_rot.y*-1.0f + D3DX_PI;
		}
	}
}
//プレイヤーの水保有量を返す
float CCharacter::GetPlayerWaterVol(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_waterVolume;
		}
	}
}
//プレイヤーのライフを返す
float CCharacter::GetPlayerLife(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return (float)m_character[teamNo][i]->m_life / (float)m_character[teamNo][i]->m_maxLife;
		}
	}
}
// プレイヤーを殺したやつの位置
D3DXVECTOR3	CCharacter::GetPlayerKillerPos(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[m_character[teamNo][i]->m_killer / 10][m_character[teamNo][i]->m_killer % 10]->m_beforePos;
		}
	}
}
// プレイヤーが生きてる
bool CCharacter::GetPlayerSurvive(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_surviveFlag;
		}
	}
}
// プレイヤーのスキルゲージ
float	CCharacter::GetPlayerSkillGauge(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_skillGauge;
		}
	}
}
// 一位のチーム番号を返す
int	CCharacter::GetWinner() {
	if (m_winner != -1)	return m_winner;
	int temp = 0;
	for (int i = 0; i < m_teamNum; i++) {
		if (m_killAllCnt[temp] == m_killAllCnt[i]) {
			if (m_deathAllCnt[temp] > m_deathAllCnt[i]) {
				temp = i;		// キル同じならデスが少ないほう
			}
			else if (m_deathAllCnt[temp] == m_deathAllCnt[i]) {
				if (rand() % 2)	temp = i;	// キルデス同じならランダムでどっちか
			}
		}
		else if (m_killAllCnt[temp] < m_killAllCnt[i]) {
			temp = i;		// キルの多いほう
		}
	}
	m_winner = temp;
	return temp;
}
void CCharacter::LoadCharacterModel()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LoadModel("data/MODEL/bucket1.x", &m_bucketModel[0]);
	LoadModel("data/MODEL/bucket2.x", &m_bucketModel[1]);

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		"data/TEXTURE/lockon.png",			// ファイルの名前
		&m_pD3DTextureLockOn);	// 読み込むメモリー
}
// キャラチェンジ
// 汚すぎるので触るときは言ってください
void CCharacter::ChangePlayer() {
	CCharacter tempFrom = *changeFrom;
	CCharacter tempTo = *changeTo;
	// パラメータ交換　コメントめんどすぎるので省略
	{
		// 中身の位置
		int tempContent = m_contentPos[changeFrom->m_teamNo][changeFrom->m_No];
		m_contentPos[changeFrom->m_teamNo][changeFrom->m_No] = m_contentPos[changeFrom->m_teamNo][changeTo->m_No];
		m_contentPos[changeFrom->m_teamNo][changeTo->m_No] = tempContent;

		changeFrom->m_accel = changeTo->m_accel;
		changeTo->m_accel = tempFrom.m_accel;
		changeFrom->m_afterPos = changeTo->m_afterPos;
		changeTo->m_afterPos = tempFrom.m_afterPos;
		changeFrom->m_avoidCoolTime = changeTo->m_avoidCoolTime;
		changeTo->m_avoidCoolTime = tempFrom.m_avoidCoolTime;
		changeFrom->m_beforePos = changeTo->m_beforePos;
		changeTo->m_beforePos = tempFrom.m_beforePos;
		changeFrom->m_deathCnt = changeTo->m_deathCnt;
		changeTo->m_deathCnt = tempFrom.m_deathCnt;
		changeFrom->m_floorDist = changeTo->m_floorDist;
		changeTo->m_floorDist = tempFrom.m_floorDist;
		changeFrom->m_invincibleTime = changeTo->m_invincibleTime;
		changeTo->m_invincibleTime = tempFrom.m_invincibleTime;
		changeFrom->m_jumpFlag = changeTo->m_jumpFlag;
		changeTo->m_jumpFlag = tempFrom.m_jumpFlag;
		changeFrom->m_killCnt = changeTo->m_killCnt;
		changeTo->m_killCnt = tempFrom.m_killCnt;
		changeFrom->m_life = changeTo->m_life;
		changeTo->m_life = tempFrom.m_life;
		changeFrom->m_maxLife = changeTo->m_maxLife;
		changeTo->m_maxLife = tempFrom.m_maxLife;
		changeFrom->m_move = changeTo->m_move;
		changeTo->m_move = tempFrom.m_move;
		changeFrom->m_moveSpeed = changeTo->m_moveSpeed;
		changeTo->m_moveSpeed = tempFrom.m_moveSpeed;
		changeFrom->m_maxMoveSpeed = changeTo->m_maxMoveSpeed;
		changeTo->m_maxMoveSpeed = tempFrom.m_maxMoveSpeed;
		changeFrom->m_mtxWorld = changeTo->m_mtxWorld;
		changeTo->m_mtxWorld = tempFrom.m_mtxWorld;
		changeFrom->m_No = changeTo->m_No;
		changeTo->m_No = tempFrom.m_No;
		changeFrom->m_rot.y = changeTo->m_rot.y;
		changeTo->m_rot = tempFrom.m_rot;
		changeFrom->m_size = changeTo->m_size;
		changeTo->m_size = tempFrom.m_size;
		changeFrom->m_type = changeTo->m_type;
		changeTo->m_type = tempFrom.m_type;
		changeFrom->m_wallDistFront = changeTo->m_wallDistFront;
		changeTo->m_wallDistFront = tempFrom.m_wallDistFront;
		changeFrom->m_water = changeTo->m_water;
		changeTo->m_water = tempFrom.m_water;
		changeFrom->m_waterVolume = changeTo->m_waterVolume;
		changeTo->m_waterVolume = tempFrom.m_waterVolume;
		changeFrom->m_mesh = tempTo.m_mesh;
		changeTo->m_mesh = tempFrom.m_mesh;
		changeFrom->m_pickUpVolume = tempTo.m_pickUpVolume;
		changeTo->m_pickUpVolume = tempFrom.m_pickUpVolume;
		changeFrom->m_pickUpTime = tempTo.m_pickUpTime;
		changeTo->m_pickUpTime = tempFrom.m_pickUpTime;
		changeFrom->m_pickUpHandle = tempTo.m_pickUpHandle;
		changeTo->m_pickUpHandle = tempFrom.m_pickUpHandle;
		changeFrom->m_bucketType = tempTo.m_bucketType;
		changeTo->m_bucketType = tempFrom.m_bucketType;
		changeFrom->m_skillGauge = tempTo.m_skillGauge;
		changeTo->m_skillGauge = tempFrom.m_skillGauge;
		changeFrom->m_skillFlag = tempTo.m_skillFlag;
		changeTo->m_skillFlag = tempFrom.m_skillFlag;

		//changeTo->m_mesh.SetAnimSpeed(1.0f);
		//changeTo->m_mesh.SwitchAnimSet(ANIM_WAIT, -1);
		//changeFrom->m_mesh.SetAnimSpeed(1.0f);
		//changeFrom->m_mesh.SwitchAnimSet(ANIM_WAIT, -1);
	}

	CCamera::ChangePlayer(tempFrom.m_teamNo, tempFrom.m_afterPos);

	// AIにしかないものを初期化
	CAi* aitempFrom = (CAi*)changeTo;
	aitempFrom->ChangeOccasion();

	// 配列位置交換
	m_character[tempFrom.m_teamNo][tempTo.m_No] = changeFrom;
	m_character[tempFrom.m_teamNo][tempFrom.m_No] = changeTo;
}

void CCharacter::ResultFunc() {
	int winner;
	winner = GetWinner();
	if (CGame::GetGameParam().stageType == 0) {
		for (int i = 0; i < m_teamNum; i++) {
			for (int j = 0; j < MAX_TEAMMEMBER; j++) {
				m_character[i][j]->m_afterPos = D3DXVECTOR3(-800, 100, 800);
				m_character[i][j]->m_beforePos = D3DXVECTOR3(-800, 100, 800);
				m_character[i][j]->m_accel = D3DXVECTOR3(0.0f, -0.1f, 0.0f);
				m_character[i][j]->m_rot.y = 9.68f;
				m_character[i][j]->m_move = D3DXVECTOR3(0, 0, 0);
			}
		}

		m_character[winner][0]->m_afterPos = D3DXVECTOR3(0, 100, 0);
		m_character[winner][1]->m_afterPos = D3DXVECTOR3(-286, 100, 33);
		m_character[winner][2]->m_afterPos = D3DXVECTOR3(-338, 100, 12);
		m_character[winner][0]->m_beforePos = D3DXVECTOR3(31, 244, -40);
		m_character[winner][1]->m_beforePos = D3DXVECTOR3(0, 244, -17);
		m_character[winner][2]->m_beforePos = D3DXVECTOR3(42, 244, -6);
	}
	else {
		for (int i = 0; i < m_teamNum; i++) {
			for (int j = 0; j < MAX_TEAMMEMBER; j++) {
				m_character[i][j]->m_afterPos = D3DXVECTOR3(-800, 100, 800);
				m_character[i][j]->m_beforePos = D3DXVECTOR3(-800, 100, 800);
				m_character[i][j]->m_accel = D3DXVECTOR3(0.0f, -0.1f, 0.0f);
				m_character[i][j]->m_rot.y = 6.9f;
				m_character[i][j]->m_move = D3DXVECTOR3(0, 0, 0);
			}
		}

		m_character[winner][0]->m_afterPos = D3DXVECTOR3(-320, 100, 25);
		m_character[winner][1]->m_afterPos = D3DXVECTOR3(-286, 100, 33);
		m_character[winner][2]->m_afterPos = D3DXVECTOR3(-338, 100, 12);
		m_character[winner][0]->m_beforePos = D3DXVECTOR3(-320, 100, 25);
		m_character[winner][1]->m_beforePos = D3DXVECTOR3(-286, 100, 33);
		m_character[winner][2]->m_beforePos = D3DXVECTOR3(-337, 100, -5.7);
	}
}

//************ メンバー関数 ****************
//コンストラクタ
CCharacter::CCharacter(int characterType, int teamNo, int bucketType) : CMeshObj()
{
	CMeshObj::Init();

	//キャラクターのタイプ
	m_type = characterType;

	//バケツのタイプ(仮)
	m_bucketType = bucketType;
	std::string s;
	const char* c;
	switch (characterType)
	{
	case 0:
		s = MODEL_CHARACTER_LOUCUST;
		break;
	case 1:
		s = MODEL_CHARACTER_FIREFLY;
		break;
	case 2:
		s = MODEL_CHARACTER_BEETLE;
		break;
	default:
		break;
	}
	s += std::to_string(teamNo + 1);
	s += ".x";
	c = s.c_str();
	m_mesh.Initialize(c);
	m_mesh.SwitchAnimSet(ANIM_WAIT, true);

	//境界ボックス
	m_size = D3DXVECTOR3(8, 11, 8);

	//チームの番号を決定する
	m_teamNo = teamNo;

	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (!m_character[teamNo][i]) {
			m_No = i;
			break;
		}
	}

	m_pickUpHandle = -1;
	m_invincibleTime = -1.0f;

	//位置・向きの初期設定
	//チーム番号に応じたスポーン位置を入れる
	switch (m_teamNo)
	{
	case 0:
		m_respawnPos = D3DXVECTOR3(-800.0f, 0.0f, 0.0f);
		if (m_No == 1)	m_respawnPos += D3DXVECTOR3(0.0f, 0.0f, -30.0f);
		else if (m_No == 2)	m_respawnPos += D3DXVECTOR3(0.0f, 0.0f, 30.0f);
		break;

	case 1:
		m_respawnPos = D3DXVECTOR3(800.0f, 0.0f, 0.0f);
		if (m_No == 1)	m_respawnPos += D3DXVECTOR3(0.0f, 0.0f, -30.0f);
		else if (m_No == 2)	m_respawnPos += D3DXVECTOR3(0.0f, 0.0f, 30.0f);
		break;

	case 2:
		m_respawnPos = D3DXVECTOR3(0.0f, 0.0f, 800.0f);
		if (m_No == 1)	m_respawnPos += D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
		else if (m_No == 2)	m_respawnPos += D3DXVECTOR3(30.0f, 0.0f, 0.0f);
		break;

	case 3:
		m_respawnPos = D3DXVECTOR3(0.0f, 0.0f, -800.0f);
		if (m_No == 1)	m_respawnPos += D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
		else if (m_No == 2)	m_respawnPos += D3DXVECTOR3(30.0f, 0.0f, 0.0f);
		break;

	default:
		break;
	}

	// キャラタイプごとの特性
	switch (m_type)
	{
	case CHARACTER_TYPE_LOCUST:
		m_maxLife = 100;
		m_skillAccumulate = SKILL_ACCUMULATE;
		m_maxMoveSpeed = PLAYER_MOVE_SPEED + 1;
		break;
	case CHARACTER_TYPE_FIREFLY:
		m_maxLife = 100;
		m_skillAccumulate = SKILL_ACCUMULATE*1.5f;
		m_maxMoveSpeed = PLAYER_MOVE_SPEED;
		break;
	case CHARACTER_TYPE_BETTLE:
		m_maxLife = 150;
		m_skillAccumulate = SKILL_ACCUMULATE;
		m_maxMoveSpeed = PLAYER_MOVE_SPEED;
		break;
	default:
		break;
	}
}

//デストラクタ
CCharacter::~CCharacter() {
	if (CGame::GetStep() != STEP_GAME)
		m_mesh.Finalize();
}

//位置とか初期化
void CCharacter::Init() {
	m_surviveFlag = true;
	m_killer = -1;
	m_skillFlag = false;
	m_skillGauge = 0.0f;
	//最大ライフ（初期ライフ）を１００に設定
	m_life = m_maxLife;
	m_beforePos = m_afterPos = m_respawnPos;

	//チームごとにカメラの向き初期化
	switch (m_teamNo)
	{
	case 0:
	{
		m_rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.5f, 0.0f);
		break;
	}
	case 1:
	{
		m_rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		break;
	}
	case 2:
	{
		m_rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f);
		break;
	}
	case 3:
	{
		m_rot = D3DXVECTOR3(0.0f, D3DX_PI * 2.0f, 0.0f);
		break;
	}
	}
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_accel = D3DXVECTOR3(0.0f, -0.1f, 0.0f);

	m_wallDistFront = 9999;
	m_waterVolume = 0.0f;
}

void CCharacter::Uninit() {

}

void CCharacter::Update() {
	// 攻撃中ならアップデート
	if (m_water && !m_water->Update()) {
		delete m_water;
		m_water = NULL;
	}

	if (RespawnCharacter()) {
		return;
	}
	m_move += m_accel;
	if (m_move.y >= 0.0f) {
		m_move.y -= 0.1f;
	}

#ifdef _DEBUG
	// デバッグ浮遊
	if (m_playerFlag) {
		if (GetPadButton(m_teamNo, PAD_FLAG_PRESS, PAD_UP_CROSS)) {
			m_beforePos.y += 2.0f;
			m_accel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_move.y = 0.0f;
		}
		if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_DOWN_CROSS)) {
			m_accel = D3DXVECTOR3(0.0f, -0.1f, 0.0f);
		}
	}
#endif

	// 壁との当たり判定
	if (CollisionWall(&m_beforePos, m_move, CStage::m_stage->m_pColStageMeshModel, &m_wallDistFront)) {}
	else {
		// 移動反映
		m_beforePos += m_move;
	}
	// フィールド外との当たり判定
	SetCollisionWall(&m_beforePos, &m_move);
	// 地形との当たり判定
	if (CollisionStage(&m_beforePos, CStage::m_stage->m_pColStageMeshModel, &m_floorDist)) {
		if (m_floorDist <= 10.0f&&m_move.y < 0.0f) {
			m_jumpFlag = false;		// 床との距離が短いときはジャンプフラグを戻す
			m_move.y = 0.0f;		// 下移動もなくす
		}
	}
	else {
		m_beforePos.y = 0;
	}


	//-----アニメーション切り替え------
	if (m_pickUpTime > 0) {

	}
	// スプラッシュ
	else if (m_water) {
	}
	// ジャンプ
	else if (m_jumpFlag == true) {
	}
	// 横歩き
	else if (m_sideStepFlag) {
		D3DXVECTOR2 a = D3DXVECTOR2(m_move.x, m_move.z);
		float b = D3DXVec2Length(&a);
		m_mesh.SetAnimSpeed(b*0.5f);
		m_mesh.SwitchAnimSet(ANIM_SIDESTEP, -1);
		m_walkSECnt += b;
	}
	// 走る
	else if (fabs(m_move.x + m_move.z) > 0.0f)
	{
		D3DXVECTOR2 a = D3DXVECTOR2(m_move.x, m_move.z);
		float b = D3DXVec2Length(&a);
		m_mesh.SetAnimSpeed(b*0.8f);
		m_mesh.SwitchAnimSet(ANIM_RUN, -1);
		m_walkSECnt += b;
		if (b > 3) {
			m_waterVolume -= b*0.0001f;
		}
	}
	// 待機
	else {
		m_mesh.SetAnimSpeed(1.0f);
		m_mesh.SwitchAnimSet(ANIM_WAIT, -1);
	}

	if (m_walkSECnt > 50) {
		// 一番近いカメラに合わせた音量設定
		float vol = 9999;
		for (int i = 0; i < CCamera::GetCameraNum(); ++i) {
			D3DXVECTOR3 dist = CCamera::GetPosCamera(i) - m_beforePos;
			float temp = D3DXVec3Length(&dist);
			if (temp < vol) {
				vol = temp;
			}
		}
		// 距離400以上は音が聞こえない
		vol = 400.0f - vol;
		if (vol > 0) {
			vol /= 400.0f;
			vol += 0.2f;
			PlaySound(SOUND_LABEL_ASIOTO1 + m_teamNo*MAX_TEAMMEMBER + m_No, vol);
		}
		m_walkSECnt = 0;
	}
	m_afterPos = m_beforePos;

	DamageLife();

	m_healCnt--;
	if (m_maxLife > m_life && m_healCnt < 0) {
		m_life++;	// ライフ自然回復
		m_healCnt = 10;
	}

	// 水すくい
	UpdatePickUp();

	// スキルゲージがたまる
	m_skillGauge += m_skillAccumulate;
	if (m_skillGauge >= 1.0f) {
		// ゲージ満タン時
		m_skillGauge = 1.0f;
	}
	m_avoidCoolTime--;
	m_invincibleTime--;
}

void CCharacter::Draw(int camNo) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxSize;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxSize, 2, 2, 2);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxSize);

	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, -1.0f*m_rot.y, m_rot.x, 0);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, m_afterPos.x, m_afterPos.y, m_afterPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	CMeshObj::Draw(m_mtxWorld);

	//マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);


	// バケツ描画
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 11.1f, 17.1f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, CMeshObj::GetHandMatrix());

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	DrawModel(&m_bucketModel[m_bucketType]);

	//弾描画
	if (m_water) {
		m_water->Draw(camNo);
	}
}

//壁の限界値設定
void SetCollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *move)
{
	D3DXVECTOR3 max;
	D3DXVECTOR3 min;
	CStage::m_stage->GetStageBound(&max, &min);
	if (pos->x > max.x)
	{
		pos->x = max.x;
		move->x = 0.0f;
	}
	else if (pos->x < min.x)
	{
		pos->x = min.x;
		move->x = 0.0f;
	}
	if (pos->z > max.z)
	{
		pos->z = max.z;
		move->z = 0.0f;
	}
	else if (pos->z < min.z)
	{
		pos->z = min.z;
		move->z = 0.0f;
	}
}

void CCharacter::ReleaseCharacterModel() {
	ReleaseModel(&m_bucketModel[0]);
	ReleaseModel(&m_bucketModel[1]);
	&m_bucketModel[0] == NULL;
	&m_bucketModel[1] == NULL;
}

//被弾ダメージ
void CCharacter::DamageLife()
{
	if (m_skillFlag)	return;
	if (m_invincibleTime >= 0.0f)	return;
	float life = (float)m_life;
	for (int i = 0; i < m_teamNum; i++) {
		//パーティクルが味方チームのものだったらスルーする
		if (m_teamNo == i)	continue;
		for (int j = 0; j < MAX_TEAMMEMBER; j++) {
			// 発射していなければスルー
			if (!m_character[i][j]->m_water)	continue;
			for (int k = 0; k < WATER_MAX_COL; k++)
			{
				int particleNum;		// 衝突グループに含まれるパーティクルの数
				D3DXVECTOR3 particlePos = m_character[i][j]->m_water->GetPos(k, &particleNum);
				if (particleNum == 0)	continue;
				//キャラクターとパーティクルが当たっていたら
				D3DXVECTOR3 len = m_beforePos - particlePos;
				if (D3DXVec3Length(&len) > 200)	break;	// 距離が大分遠ければ全部スルー
				if (CollisionAABB(D3DXVECTOR3(m_afterPos.x, m_afterPos.y + m_size.y*0.5f, m_afterPos.z), m_size, particlePos, D3DXVECTOR3(20, 20, 20)))
				{
					//当たったパーティクルの数をカウントする
					life -= particleNum*0.5f;
					m_skillGauge += particleNum*0.0025f;
					//当たった分のパーティクルを削除
					m_character[i][j]->m_water->SetUseDown(k);
					// 死んだら
					if (life <= 0.0f) {
						m_life = life;
						//Init();
						m_killer = i * 10 + j;
						Kill(m_killer);
						return;
					}
				}
			}
		}
	}
	m_life = life;
}

//リスポーン処理
bool CCharacter::RespawnCharacter()
{
	//倒されたキャラクターはリスポーン
	if (!m_surviveFlag)
	{
		if (m_rot.x < D3DX_PI*0.5f) {
			m_rot.x += 0.05f;
		}
		m_respawnTime--;
		if (m_respawnTime <= 0) {
			m_invincibleTime = 180;
			m_surviveFlag = true;
			m_rot.x = 0.0f;
			Init();
		}
		return true;
	}
	return false;
}

void CCharacter::Kill(int killerNo) {
	m_killer = killerNo;
	m_surviveFlag = false;
	m_respawnTime = RESPAWNTIME;
	m_mesh.SwitchAnimSet(ANIM_RUN, 0);
	m_character[killerNo / 10][killerNo % 10]->KillCountAdd();	// 倒した側のキルカウントＵＰ
	DeathCountAdd();					// 死んだカウントUP
}
// キルカウントを増やす
void CCharacter::KillCountAdd() {
	m_killAllCnt[m_teamNo]++;
	m_killCnt++;
}

// デスカウントを増やす
void CCharacter::DeathCountAdd() {
	m_deathAllCnt[m_teamNo]++;
	m_deathCnt++;
}

// 水をすくうときに呼ぶ
void CCharacter::PickUpWater() {
	// 二秒間すくいなおせない
	if (m_pickUpTime > -30)	return;
	if (m_waterVolume < 1.0f) {
		for (int i = 0; i < CWaterPuddle::m_waterPuddle.size(); i++) {
			//水たまりの上にプレイヤーがいたら
			if (CollisionAABB(m_afterPos, m_size, CWaterPuddle::GetWaterPuddlePos(i), CWaterPuddle::GetWaterPuddleSize(i))) {
				m_pickUpHandle = CWaterPuddle::GetWaterHandle(i);
				if (CWaterPuddle::GetWaterPuddleVolume(i) >= 1.0f - m_waterVolume) {
					m_pickUpVolume = (1.0f - m_waterVolume);
				}
				else {
					m_pickUpVolume = CWaterPuddle::GetWaterPuddleVolume(i);
				}
				m_pickUpTime = PICKUP_TIME;
				m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				// すくうアニメーション
				m_mesh.SetAnimSpeed(2.0f);
				m_mesh.SetTime(0.0f);
				m_mesh.SwitchAnimSet(ANIM_PICKUP, 2.0f);
				break;
			}
		}
	}
}

// 水をすくうアップデート
void CCharacter::UpdatePickUp() {
	if (m_pickUpVolume != 0.0f&&m_pickUpTime <= 0) {
		CWaterPuddle::SubRemain2(m_pickUpHandle, m_pickUpVolume);
		m_waterVolume += m_pickUpVolume;
		m_pickUpHandle = -1;
		m_pickUpVolume = 0.0f;
	}
	else {
		m_pickUpTime--;
	}
}

