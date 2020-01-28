///////////////////////////////////////////////////
//
//			�L�����N�^�[
//
//////////////////////////////////////////////////
//----------------�C���N���[�h--------------
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
//-------------�萔�}�N��--------------
#define RESPAWNTIME	(120)	// ���X�|���܂ł̎���
#define PICKUP_TIME	(30)	// �����������ő厞��
#define PLAYER_MOVE_SPEED (4)
#define SKILL_ACCUMULATE	(1.0f/3600.0f)			// �ꕪ�ԂŃX�L���Q�[�W�}�b�N�X
//---------�ÓI�����o-----------
CCharacter*				CCharacter::m_character[MAX_TEAM][MAX_TEAMMEMBER];
int						CCharacter::m_teamNum;											//�`�[����

int						CCharacter::m_killAllCnt[MAX_TEAM];								// �`�[���̃g�[�^���L���J�E���g
int						CCharacter::m_deathAllCnt[MAX_TEAM];							// �`�[���̃g�[�^���f�X�J�E���g

CCharacter*				CCharacter::changeFrom = NULL;									// �`�F���W��
CCharacter*				CCharacter::changeTo = NULL;									// �`�F���W��
										// ���b�V��
int						CCharacter::m_contentPos[MAX_TEAM][MAX_TEAMMEMBER] = { {0,1,2},{0,1,2},{0,1,2},{0,1,2} };
void SetCollisionWall(D3DXVECTOR3*, D3DXVECTOR3*);

int CCharacter::m_winner;

MODEL					CCharacter::m_bucketModel[2] = { NULL,NULL };
LPDIRECT3DTEXTURE9		CCharacter::m_pD3DTextureLockOn;
//---------------�����o�֐�---------------

//************ static�֐� *******************
//�L�����N�^�[�쐬
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
//�S�X�V
void CCharacter::AllUpdate() {
	for (int i = 0; i < m_teamNum*MAX_TEAMMEMBER; i++) {
		m_character[0][i]->Update();
		if (i % MAX_TEAMMEMBER == 2) {
			// �`�F���W
			if (changeFrom) {
				ChangePlayer();
				changeFrom = NULL;
				changeTo = NULL;
			}
		}
	}
}
//�S�`��
void CCharacter::AllDraw(int camNo) {
	for (int i = 0; i < m_teamNum; i++) {
		if (CGame::GetStep() == STEP_FINISH&&EndFadeOut()) {
			if (GetWinner() != i)	continue;
		}
		for (int j = 0; j < MAX_TEAMMEMBER; j++)
			m_character[i][j]->Draw(camNo);
	}
}
//�v���C���[�̈ʒu��Ԃ�
//�����J�����Ŏg������
D3DXVECTOR3 CCharacter::GetPlayerPos(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_afterPos;
		}
	}
}
//�v���C���[�̈ړ��ʂ�Ԃ�
D3DXVECTOR3 CCharacter::GetPlayerMove(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_move;
		}
	}
}
//�v���C���[�̉�]������Ԃ�
float CCharacter::GetPlayerRot(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_rot.y*-1.0f + D3DX_PI;
		}
	}
}
//�v���C���[�̐��ۗL�ʂ�Ԃ�
float CCharacter::GetPlayerWaterVol(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_waterVolume;
		}
	}
}
//�v���C���[�̃��C�t��Ԃ�
float CCharacter::GetPlayerLife(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return (float)m_character[teamNo][i]->m_life / (float)m_character[teamNo][i]->m_maxLife;
		}
	}
}
// �v���C���[���E������̈ʒu
D3DXVECTOR3	CCharacter::GetPlayerKillerPos(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[m_character[teamNo][i]->m_killer / 10][m_character[teamNo][i]->m_killer % 10]->m_beforePos;
		}
	}
}
// �v���C���[�������Ă�
bool CCharacter::GetPlayerSurvive(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_surviveFlag;
		}
	}
}
// �v���C���[�̃X�L���Q�[�W
float	CCharacter::GetPlayerSkillGauge(int teamNo) {
	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (m_character[teamNo][i]->m_playerFlag) {
			return m_character[teamNo][i]->m_skillGauge;
		}
	}
}
// ��ʂ̃`�[���ԍ���Ԃ�
int	CCharacter::GetWinner() {
	if (m_winner != -1)	return m_winner;
	int temp = 0;
	for (int i = 0; i < m_teamNum; i++) {
		if (m_killAllCnt[temp] == m_killAllCnt[i]) {
			if (m_deathAllCnt[temp] > m_deathAllCnt[i]) {
				temp = i;		// �L�������Ȃ�f�X�����Ȃ��ق�
			}
			else if (m_deathAllCnt[temp] == m_deathAllCnt[i]) {
				if (rand() % 2)	temp = i;	// �L���f�X�����Ȃ烉���_���łǂ�����
			}
		}
		else if (m_killAllCnt[temp] < m_killAllCnt[i]) {
			temp = i;		// �L���̑����ق�
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

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		"data/TEXTURE/lockon.png",			// �t�@�C���̖��O
		&m_pD3DTextureLockOn);	// �ǂݍ��ރ������[
}
// �L�����`�F���W
// ��������̂ŐG��Ƃ��͌����Ă�������
void CCharacter::ChangePlayer() {
	CCharacter tempFrom = *changeFrom;
	CCharacter tempTo = *changeTo;
	// �p�����[�^�����@�R�����g�߂�ǂ�����̂ŏȗ�
	{
		// ���g�̈ʒu
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

	// AI�ɂ����Ȃ����̂�������
	CAi* aitempFrom = (CAi*)changeTo;
	aitempFrom->ChangeOccasion();

	// �z��ʒu����
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

//************ �����o�[�֐� ****************
//�R���X�g���N�^
CCharacter::CCharacter(int characterType, int teamNo, int bucketType) : CMeshObj()
{
	CMeshObj::Init();

	//�L�����N�^�[�̃^�C�v
	m_type = characterType;

	//�o�P�c�̃^�C�v(��)
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

	//���E�{�b�N�X
	m_size = D3DXVECTOR3(8, 11, 8);

	//�`�[���̔ԍ������肷��
	m_teamNo = teamNo;

	for (int i = 0; i < MAX_TEAMMEMBER; i++) {
		if (!m_character[teamNo][i]) {
			m_No = i;
			break;
		}
	}

	m_pickUpHandle = -1;
	m_invincibleTime = -1.0f;

	//�ʒu�E�����̏����ݒ�
	//�`�[���ԍ��ɉ������X�|�[���ʒu������
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

	// �L�����^�C�v���Ƃ̓���
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

//�f�X�g���N�^
CCharacter::~CCharacter() {
	if (CGame::GetStep() != STEP_GAME)
		m_mesh.Finalize();
}

//�ʒu�Ƃ�������
void CCharacter::Init() {
	m_surviveFlag = true;
	m_killer = -1;
	m_skillFlag = false;
	m_skillGauge = 0.0f;
	//�ő僉�C�t�i�������C�t�j���P�O�O�ɐݒ�
	m_life = m_maxLife;
	m_beforePos = m_afterPos = m_respawnPos;

	//�`�[�����ƂɃJ�����̌���������
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
	// �U�����Ȃ�A�b�v�f�[�g
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
	// �f�o�b�O���V
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

	// �ǂƂ̓����蔻��
	if (CollisionWall(&m_beforePos, m_move, CStage::m_stage->m_pColStageMeshModel, &m_wallDistFront)) {}
	else {
		// �ړ����f
		m_beforePos += m_move;
	}
	// �t�B�[���h�O�Ƃ̓����蔻��
	SetCollisionWall(&m_beforePos, &m_move);
	// �n�`�Ƃ̓����蔻��
	if (CollisionStage(&m_beforePos, CStage::m_stage->m_pColStageMeshModel, &m_floorDist)) {
		if (m_floorDist <= 10.0f&&m_move.y < 0.0f) {
			m_jumpFlag = false;		// ���Ƃ̋������Z���Ƃ��̓W�����v�t���O��߂�
			m_move.y = 0.0f;		// ���ړ����Ȃ���
		}
	}
	else {
		m_beforePos.y = 0;
	}


	//-----�A�j���[�V�����؂�ւ�------
	if (m_pickUpTime > 0) {

	}
	// �X�v���b�V��
	else if (m_water) {
	}
	// �W�����v
	else if (m_jumpFlag == true) {
	}
	// ������
	else if (m_sideStepFlag) {
		D3DXVECTOR2 a = D3DXVECTOR2(m_move.x, m_move.z);
		float b = D3DXVec2Length(&a);
		m_mesh.SetAnimSpeed(b*0.5f);
		m_mesh.SwitchAnimSet(ANIM_SIDESTEP, -1);
		m_walkSECnt += b;
	}
	// ����
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
	// �ҋ@
	else {
		m_mesh.SetAnimSpeed(1.0f);
		m_mesh.SwitchAnimSet(ANIM_WAIT, -1);
	}

	if (m_walkSECnt > 50) {
		// ��ԋ߂��J�����ɍ��킹�����ʐݒ�
		float vol = 9999;
		for (int i = 0; i < CCamera::GetCameraNum(); ++i) {
			D3DXVECTOR3 dist = CCamera::GetPosCamera(i) - m_beforePos;
			float temp = D3DXVec3Length(&dist);
			if (temp < vol) {
				vol = temp;
			}
		}
		// ����400�ȏ�͉����������Ȃ�
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
		m_life++;	// ���C�t���R��
		m_healCnt = 10;
	}

	// ��������
	UpdatePickUp();

	// �X�L���Q�[�W�����܂�
	m_skillGauge += m_skillAccumulate;
	if (m_skillGauge >= 1.0f) {
		// �Q�[�W���^����
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

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxSize, 2, 2, 2);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxSize);

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, -1.0f*m_rot.y, m_rot.x, 0);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_afterPos.x, m_afterPos.y, m_afterPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	CMeshObj::Draw(m_mtxWorld);

	//�}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);


	// �o�P�c�`��
	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 11.1f, 17.1f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, CMeshObj::GetHandMatrix());

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	DrawModel(&m_bucketModel[m_bucketType]);

	//�e�`��
	if (m_water) {
		m_water->Draw(camNo);
	}
}

//�ǂ̌��E�l�ݒ�
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

//��e�_���[�W
void CCharacter::DamageLife()
{
	if (m_skillFlag)	return;
	if (m_invincibleTime >= 0.0f)	return;
	float life = (float)m_life;
	for (int i = 0; i < m_teamNum; i++) {
		//�p�[�e�B�N���������`�[���̂��̂�������X���[����
		if (m_teamNo == i)	continue;
		for (int j = 0; j < MAX_TEAMMEMBER; j++) {
			// ���˂��Ă��Ȃ���΃X���[
			if (!m_character[i][j]->m_water)	continue;
			for (int k = 0; k < WATER_MAX_COL; k++)
			{
				int particleNum;		// �Փ˃O���[�v�Ɋ܂܂��p�[�e�B�N���̐�
				D3DXVECTOR3 particlePos = m_character[i][j]->m_water->GetPos(k, &particleNum);
				if (particleNum == 0)	continue;
				//�L�����N�^�[�ƃp�[�e�B�N�����������Ă�����
				D3DXVECTOR3 len = m_beforePos - particlePos;
				if (D3DXVec3Length(&len) > 200)	break;	// �������啪������ΑS���X���[
				if (CollisionAABB(D3DXVECTOR3(m_afterPos.x, m_afterPos.y + m_size.y*0.5f, m_afterPos.z), m_size, particlePos, D3DXVECTOR3(20, 20, 20)))
				{
					//���������p�[�e�B�N���̐����J�E���g����
					life -= particleNum*0.5f;
					m_skillGauge += particleNum*0.0025f;
					//�����������̃p�[�e�B�N�����폜
					m_character[i][j]->m_water->SetUseDown(k);
					// ���񂾂�
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

//���X�|�[������
bool CCharacter::RespawnCharacter()
{
	//�|���ꂽ�L�����N�^�[�̓��X�|�[��
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
	m_character[killerNo / 10][killerNo % 10]->KillCountAdd();	// �|�������̃L���J�E���g�t�o
	DeathCountAdd();					// ���񂾃J�E���gUP
}
// �L���J�E���g�𑝂₷
void CCharacter::KillCountAdd() {
	m_killAllCnt[m_teamNo]++;
	m_killCnt++;
}

// �f�X�J�E���g�𑝂₷
void CCharacter::DeathCountAdd() {
	m_deathAllCnt[m_teamNo]++;
	m_deathCnt++;
}

// �����������Ƃ��ɌĂ�
void CCharacter::PickUpWater() {
	// ��b�Ԃ������Ȃ����Ȃ�
	if (m_pickUpTime > -30)	return;
	if (m_waterVolume < 1.0f) {
		for (int i = 0; i < CWaterPuddle::m_waterPuddle.size(); i++) {
			//�����܂�̏�Ƀv���C���[��������
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
				// �������A�j���[�V����
				m_mesh.SetAnimSpeed(2.0f);
				m_mesh.SetTime(0.0f);
				m_mesh.SwitchAnimSet(ANIM_PICKUP, 2.0f);
				break;
			}
		}
	}
}

// �����������A�b�v�f�[�g
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

