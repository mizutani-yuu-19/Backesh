///////////////////////////////////////////////////
//
// �X�e�[�W
//
//////////////////////////////////////////////////
//----------------�C���N���[�h--------------
#include "stage.h"
#include "water_puddle.h"

CStage* CStage::m_stage;				// ����
D3DXVECTOR3 CStage::m_missingPoint[MAX_MISSING_POINT];

// �X�e�[�W�쐬
// �����F�X�e�[�W�ԍ��O���͂P
void CStage::CreateStage(int type) {
	m_stage = new CStage(type);
}

// �R���X�g���N�^
CStage::CStage(int type) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	switch (type)
	{
	case 0:
		// �X�e�[�W1
		// �Փ˔��胂�f��
		D3DXLoadMeshFromX(MODEL_COL_STAGE1,
			D3DXMESH_SYSTEMMEM, pDevice, NULL,
			&m_pColStageBuffMatModel, NULL,
			&m_nColStageNumMatModel,
			&m_pColStageMeshModel);
		// �����ڃ��f��
		LoadModel(MODEL_STAGE1, &m_showStageModel);

		//�G�T�[�`���s���ɗ^���郉���_�����W
		m_missingPoint[0] = D3DXVECTOR3(-240, 0, -820);
		m_missingPoint[1] = D3DXVECTOR3(-350, 0, -325);
		m_missingPoint[2] = D3DXVECTOR3(-185, 0, 700);
		m_missingPoint[3] = D3DXVECTOR3(110, 0, -555);
		m_missingPoint[4] = D3DXVECTOR3(240, 0, 140);
		m_missingPoint[5] = D3DXVECTOR3(330, 0, -825);
		m_missingPoint[6] = D3DXVECTOR3(540, 0, 450);
		m_missingPoint[7] = D3DXVECTOR3(700, 0, 50);

		//�Q�[���X�^�[�g�����琶������Ă��鐅���܂���
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(700.0f, 0.0f, 700.0f), 1.0f, true);
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(700.0f, 0.0f, -700.0f), 1.0f, true);
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(-700.0f, 0.0f, 700.0f), 1.0f, true);
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(-700.0f, 0.0f, -700.0f), 1.0f, true);
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(-300.0f, 0.0f, -200.0f), 0.5f, true);
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(300.0f, 0.0f, 200.0f), 0.5f, true);


		break;
	case 1:
		// �X�e�[�W�Q
		// �Փ˔��胂�f��
		D3DXLoadMeshFromX(MODEL_COL_STAGE2,
			D3DXMESH_SYSTEMMEM, pDevice, NULL,
			&m_pColStageBuffMatModel, NULL,
			&m_nColStageNumMatModel,
			&m_pColStageMeshModel);
		// �����ڃ��f��
		LoadModel(MODEL_STAGE2, &m_showStageModel);

		//�G�T�[�`���s���ɗ^���郉���_�����W
		m_missingPoint[0] = D3DXVECTOR3(-330, 0, 250);
		m_missingPoint[1] = D3DXVECTOR3(-50, 0, 590);
		m_missingPoint[2] = D3DXVECTOR3(205, 0, 110);
		m_missingPoint[3] = D3DXVECTOR3(715, 0, 465);
		m_missingPoint[4] = D3DXVECTOR3(200, 0, -285);
		m_missingPoint[5] = D3DXVECTOR3(-265, 0, 305);
		m_missingPoint[6] = D3DXVECTOR3(-700, 0, -400);
		m_missingPoint[7] = D3DXVECTOR3(400, 0, -650);

		//�Q�[���X�^�[�g�����琶������Ă��鐅���܂���
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(600.0f, 0.0f, -600.0f), 1.0f, true);
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(-600.0f, 0.0f, 600.0f), 1.0f, true);
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(-600.0f, 0.0f, -600.0f), 1.0f, true);
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(350.0f, 0.0f, 200.0f), 0.8f, true);
		CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3(-100.0f, 0.0f, 50.0f), 0.5f, true);

	default:
		break;
	}
	//���E�{�b�N�X�Z�o
	LPDIRECT3DVERTEXBUFFER9  vertexBuffer;
	void*                    vertex;
	D3DXVECTOR3				min;
	D3DXVECTOR3				max;
	m_pColStageMeshModel->GetVertexBuffer(&vertexBuffer);
	vertexBuffer->Lock(0, 0, &vertex, 0);

	D3DXComputeBoundingBox((D3DXVECTOR3*)vertex,
		m_pColStageMeshModel->GetNumVertices(),
		D3DXGetFVFVertexSize(m_pColStageMeshModel->GetFVF()),
		&min, &max);

	vertexBuffer->Unlock();
	vertexBuffer->Release();

	m_max = max;
	m_min = min;

	m_max -= D3DXVECTOR3(5.5f, 5.5f, 5.5f);
	m_min += D3DXVECTOR3(5.5f, 5.5f, 5.5f);

	m_size = max - min;
}

// �f�X�g���N�^
CStage::~CStage() {
	SAFE_RELEASE(m_pColStageBuffMatModel);
	SAFE_RELEASE(m_pColStageMeshModel);
	ReleaseModel(&m_showStageModel);
}

//-----------�`��--------------
void CStage::Draw(int camNo) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	DrawModel(&m_showStageModel);
}

void CStage::GetStageBound(D3DXVECTOR3 *max, D3DXVECTOR3 *min) {
	*max = m_max;
	*min = m_min;
	return;
}


D3DXVECTOR3 CStage::GetStageSize() {
	return m_size;
}