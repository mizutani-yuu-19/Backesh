///////////////////////////////////////////////////
//
// �Q�[���V�[��
//
//////////////////////////////////////////////////
//----------------�C���N���[�h--------------
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

//--------------�萔�}�N��-----------
ST_GAMEPARAM CGame::m_gameParam;
int		CGame::m_gameTime;
int		CGame::m_step = STEP_STAGEVIEW;

//---------------�����o�֐�---------------
// �R���X�g���N�^
CGame::CGame()
{
//	�Q�[���p���̏�����
	//m_gameParam.stageType = 1;		// �X�e�[�W�̎��
	//m_gameParam.playerNum = 4;		// �v���C�l��
	//m_gameParam.teamNum = 4;		// �`�[���̐�
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

// �f�X�g���N�^
CGame::~CGame() {

	Uninit();
}

void CGame::Init() {
	m_gameTime = GAME_TIME;
	//�X�e�[�W�̐���
	CStage::CreateStage(m_gameParam.stageType);
	CSkydome::CreateSkydome(m_gameParam.stageType);

	//�L�����N�^�̐���
	CCharacter::CreateCharacter(m_gameParam.characterType, m_gameParam.playerNum, m_gameParam.teamNum, m_gameParam.bucketType);

	//�L�����N�^�̏�����
	for (int i = 0; i < CCharacter::m_teamNum; i++)
	{
		for (int j = 0; j < MAX_TEAMMEMBER; j++)
		{
			CCharacter::m_character[i][j]->Init();
		}
	}
	CEffect::LoadEffect();

	CUI::AllInit();
	// ���C�g�̏���������
	InitLight();

	// �ۉe�̏���������
	//InitShadow();

	PlaySound(SOUND_LABEL_BATTLE, 1.0f);
}

void CGame::Uninit() {
	// �L�����N�^�[�S����
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
	// �J�����̏I������
	CCamera::DeleteCamera();
	SAFE_DELETE(CStage::m_stage);
	// ���C�g�̏I������
	UninitLight();
	CEffect::Uninit();
	// �ۉe�̏I������
	//UninitShadow();
}

void CGame::Update() {
	CEffect::Update();
	// �L�����N�^�[�̍X�V
	CCharacter::AllUpdate();
	// �����܂�̍X�V
	CWaterPuddle::UpdateWaterPuddle();
	// �J�����̍X�V����
	CCamera::UpdateCamera();

	// �����̍X�V����
	UpdateLight();

	// �ۉe�̍X�V����
	UpdateShadow();

	CUI::AllUpdate();
	// �Q�[���̃^�C�}�[�����炷
	if (m_step == STEP_GAME)
		m_gameTime--;
	if (m_gameTime == 0) {
		// �����Ń��U���g�ֈړ�
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

		// �J�����̐ݒ菈��
		CCamera::SetCamera(i);

		CSkydome::m_skydome->Draw(i);

		CStage::m_stage->Draw(i);

		// �ۉe�̕`�揈��
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