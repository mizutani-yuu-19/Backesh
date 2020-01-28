//=============================================================================
//
// �J�������� [camera.cpp]
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "game_pad.h"
#include "character.h"
#include "fade.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOOKAT_PT			(100.0f)				// �����_�̃L�����N�^�[����̋���	
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
std::vector<CCamera*>	CCamera::m_camera;

//*****************************************************************************
// �v���C���[�J�����N���X
//*****************************************************************************
//===========================================================================
//	�J���������o�֐�
//===========================================================================
// �R���X�g���N�^
CPlayerCamera::CPlayerCamera(int playerNum, int playerNo)
{
	m_posCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �X�N���[����̔z�u
	// ��l
	if (playerNum == 1) {
		m_port.X = 0;
		m_port.Y = 0;
		m_port.Width = SCREEN_WIDTH;
		m_port.Height = SCREEN_HEIGHT;
		m_aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	}
	// ��l
	else if (playerNum == 2) {
		m_port.X = 0;
		m_port.Y = SCREEN_CENTER_Y*playerNo;
		m_port.Width = SCREEN_WIDTH;
		m_port.Height = SCREEN_CENTER_Y;
		m_aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT * 2;
	}
	// �O�l���͎l�l
	else if (playerNum == 3 || playerNum == 4) {
		m_port.X = SCREEN_CENTER_X*(playerNo % 2);
		m_port.Y = SCREEN_CENTER_Y*(playerNo / 2);
		m_port.Width = SCREEN_CENTER_X;
		m_port.Height = SCREEN_CENTER_Y;
		m_aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	}

	m_port.MaxZ = 1;
	m_port.MinZ = 0;

	m_moveSpeed = 0.1f;
	m_radX = D3DX_PI * 1.5f;
	m_radY = 0.0f;
	m_dest = 200.0f;

	m_No = m_camera.size();		// ���ڂ̃J����

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		VIEW_ANGLE,										// ����p
		m_aspect,	// �A�X�y�N�g��
		VIEW_NEAR_Z,									// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);									// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	GetDevice()->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
	m_radX = CCharacter::GetPlayerRot(m_No);

	m_changeTime = 999;
}


//=============================================================================
// �J�����̏�����
//=============================================================================
void CPlayerCamera::Init(void)
{

}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CPlayerCamera::Uninit(void)
{
}
//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CPlayerCamera::Update(void)
{
	D3DXVECTOR3 pos;
	// �`�F���W��
	if (m_changeTime < 10) {
		pos = CCharacter::GetPlayerPos(m_No);
		D3DXVECTOR3 cameraPos;
		D3DXVec3Lerp(&cameraPos, &m_changeFromPos, &CCharacter::GetPlayerPos(m_No), (float)m_changeTime*0.1f);
		// �����_�̈ʒu
		m_lookatPt.x = pos.x + -LOOKAT_PT * sin(m_radX);
		m_lookatPt.y = pos.y;
		m_lookatPt.z = pos.z + -LOOKAT_PT * cos(m_radX);

		// ���_�̈ʒu
		m_cameraPt.x = cameraPos.x + m_dest * sin(m_radX);
		m_cameraPt.y = cameraPos.y + 50.0f + 100 * sin(m_radY);
		m_cameraPt.z = cameraPos.z + m_dest * cos(m_radX);

		m_changeTime++;
	}
	else {
		// ���f���̈ʒu�擾
		if (CCharacter::GetPlayerSurvive(m_No)) {
			pos = CCharacter::GetPlayerPos(m_No);
		}
		else {
			pos = CCharacter::GetPlayerKillerPos(m_No);
			m_lookatPt.x = pos.x;
			m_lookatPt.y = pos.y;
			m_lookatPt.z = pos.z;
			return;
		}
		if (CGame::GetStep() == STEP_CHARACTERVIEW) {
			m_radX += 0.03f;
			if (m_radX > CCharacter::GetPlayerRot(m_No) + D3DX_PI * 2) {
				m_radX == CCharacter::GetPlayerRot(m_No);
				CGame::SetStep(STEP_COUNTDOWN);
			}
		}
		else {
			m_radX = CCharacter::GetPlayerRot(m_No);
			m_radY += GetPadRIGHTStickY(m_No)*m_moveSpeed;
			if (DIGetKey(DIK_UPARROW)) {
				m_radY += m_moveSpeed;
			}
			if (DIGetKey(DIK_DOWN)) {
				m_radY -= m_moveSpeed;
			}
		}

		if (m_radY > D3DX_PI*0.4f) {
			m_radY = D3DX_PI*0.4f;
		}
		else if (m_radY < -D3DX_PI*0.02f) {
			m_radY = -D3DX_PI*0.02f;
		}

		// �����_�̈ʒu
		m_lookatPt.x = pos.x + -LOOKAT_PT * sin(m_radX);
		m_lookatPt.y = pos.y;
		m_lookatPt.z = pos.z + -LOOKAT_PT * cos(m_radX);

		// ���_�̈ʒu
		m_cameraPt.x = m_lookatPt.x + m_dest * sin(m_radX);
		m_cameraPt.y = m_lookatPt.y + 50.0f + 100 * sin(m_radY);
		m_cameraPt.z = m_lookatPt.z + m_dest * cos(m_radX);
	}
}
void CCamera::ChangePlayer(int camNo, D3DXVECTOR3 fromPos) {
	m_camera[camNo]->m_changeFromPos = fromPos;
	m_camera[camNo]->m_changeTime = 1;
}
//*****************************************************************************
//
// �X�e�[�W�J�����N���X
//
//*****************************************************************************
//===========================================================================
//	�J���������o�֐�
//===========================================================================
// �R���X�g���N�^
CStageCamera::CStageCamera()
{
	m_posCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_port.X = 0;
	m_port.Y = 0;
	m_port.Width = SCREEN_WIDTH;
	m_port.Height = SCREEN_HEIGHT;
	m_aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	m_port.MaxZ = 1;
	m_port.MinZ = 0;

	m_moveSpeed = 0.01f;
	m_dest = 500.0f;
	m_radX = 0.0f;

	m_No = m_camera.size();		// ���ڂ̃J����

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		VIEW_ANGLE,										// ����p
		m_aspect,	// �A�X�y�N�g��
		VIEW_NEAR_Z,									// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);									// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	GetDevice()->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	m_time = 0.0f;
}


//=============================================================================
// �J�����̏�����
//=============================================================================
void CStageCamera::Init(void)
{

}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CStageCamera::Uninit(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CStageCamera::Update(void)
{
	// ���f���̈ʒu�擾
	D3DXVECTOR3 pos = CCharacter::GetPlayerPos(m_No);

	m_radX += m_moveSpeed;

	// �����_�̈ʒu
	m_lookatPt.x = 0.0f;
	m_lookatPt.y = 100.0f;
	m_lookatPt.z = 0.0f;

	// ���_�̈ʒu
	m_cameraPt.x = m_lookatPt.x + m_dest * sin(m_radX);
	m_cameraPt.y = m_lookatPt.y + 500.0f;
	m_cameraPt.z = m_lookatPt.z + m_dest * cos(m_radX);

	if (m_time > 3 * 60.0f) {
		// �J��������
		if (EndFadeOut())
			CreatePlayerCamera(CGame::GetGameParam().playerNum);
		else if (EndFadeIn())
			StartFadeOut();
	}
	m_time++;
}

//*****************************************************************************
//
// ���U���g�J�����N���X
//
//*****************************************************************************
//===========================================================================
//	�J���������o�֐�
//===========================================================================
// �R���X�g���N�^
CResultCamera::CResultCamera()
{
	m_posCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_port.X = 0;
	m_port.Y = 0;
	m_port.Width = SCREEN_WIDTH;
	m_port.Height = SCREEN_HEIGHT;
	m_aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	m_port.MaxZ = 1;
	m_port.MinZ = 0;

	m_moveSpeed = 0.01f;
	m_dest = 500.0f;
	m_radX = 0.0f;

	m_No = m_camera.size();		// ���ڂ̃J����

	if (CGame::GetGameParam().stageType == 0) {
		m_lookatPt = D3DXVECTOR3(0, 144, 62);
		m_cameraPt = D3DXVECTOR3(57, 188, -129);
	}
	else {
		m_lookatPt = D3DXVECTOR3(-254, 0, -65);
		m_cameraPt = D3DXVECTOR3(-375, 43, 93);
	}

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		VIEW_ANGLE,										// ����p
		m_aspect,	// �A�X�y�N�g��
		VIEW_NEAR_Z,									// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);									// �r���[���ʂ�FarZ�l

														// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	GetDevice()->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	m_time = 0.0f;
}


//=============================================================================
// �J�����̏�����
//=============================================================================
void CResultCamera::Init(void)
{

}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CResultCamera::Uninit(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CResultCamera::Update(void)
{
	if (m_time > 3 * 60.0f) {
		if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_A)) {
			StartFadeOut();
		}
	}
}


//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_cameraPt,		// �J�����̎��_
		&m_lookatPt,		// �J�����̒����_
		&m_posCameraU);		// �J�����̏����

	pDevice->SetViewport(&m_port);

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

CCamera::~CCamera() {

}

//*******************************************************************************************
//static
//*******************************************************************************************
//=============================================================================
// �J�����̌����̎擾
//=============================================================================
float CCamera::GetRotCamera(int no)
{
	return m_camera[no]->m_radX;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
void CCamera::GetViewMtx(D3DXMATRIX* pMtxView, int camNo)
{
	if (pMtxView) *pMtxView = m_camera[camNo]->m_mtxView;
}

void CCamera::UpdateCamera() {
	for (int i = 0; i < m_camera.size(); i++) {
		m_camera[i]->Update();
	}
}

void CCamera::CreatePlayerCamera(int playerNum) {
	for (int i = 0; i < m_camera.size(); i++) {
		delete m_camera[i];
	}
	m_camera.clear();
	for (int i = 0; i < playerNum; i++) {
		CCamera *temp = new CPlayerCamera(playerNum, i);
		m_camera.push_back(temp);
	}
	StartFadeIn();
	CGame::SetStep(CGame::GetStep() + 1);
}

void CCamera::CreateStageCamera() {
	CCamera *temp = new CStageCamera();
	m_camera.push_back(temp);
}

void CCamera::CreateResultCamera() {
	CCamera *temp = new CResultCamera();
	m_camera.push_back(temp);
}

void CCamera::DeleteCamera() {
	for (int i = 0; i < m_camera.size(); i++) {
		m_camera[i]->Uninit();
		delete m_camera[i];
	}
	m_camera.clear();
}

// �J�����Z�b�g
void CCamera::SetCamera(int camNo) {
	m_camera[camNo]->SetCamera();
}
