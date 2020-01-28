///////////////////////////////////////////////////
//
// �X�J�C�h�[��
//
//////////////////////////////////////////////////
//----------------�C���N���[�h--------------
#include "skydome.h"
#include "camera.h"

CSkydome* CSkydome::m_skydome;				// ����

// �X�e�[�W�쐬
// �����F�X�e�[�W�ԍ��O���͂P
void CSkydome::CreateSkydome(int type) {
	m_skydome = new CSkydome(type);
}

void CSkydome::DeleteSkydome() {
	SAFE_DELETE(m_skydome);
}

// �R���X�g���N�^
CSkydome::CSkydome(int type) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	switch (type)
	{
	case 0:
		// �X�e�[�W1
		// �����ڃ��f��
		LoadModel(MODEL_SKYDOME1, &m_model);
		break;
	case 1:
		// �X�e�[�W1
		// �����ڃ��f��
		LoadModel(MODEL_SKYDOME2, &m_model);
		break;
	default:
		break;
	}
}

// �f�X�g���N�^
CSkydome::~CSkydome() {
	ReleaseModel(&m_model);
}

//-----------�`��--------------
void CSkydome::Draw(int camNo) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl,mtxTrans;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXVECTOR3 camPos = CCamera::GetPosCamera(camNo);

	//�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, 20, 20, 20);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, camPos.x, camPos.y, camPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);  
	DrawModel(&m_model);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

