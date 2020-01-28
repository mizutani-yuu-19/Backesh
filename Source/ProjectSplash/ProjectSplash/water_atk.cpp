//=============================================================================
//
// ���f������ [water_atk.cpp]
//
//=============================================================================
#include "input.h"
#include "water_atk.h"
#include "camera.h"
#include "water_puddle.h"
#include "collision.h"
#include "stage.h"
#include "sound.h"
#include "sprite.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WATER			"data/texture/water.png"

#define VALUE_ROTATE		(D3DX_PI * 0.02f)

#define MAX_WATER			(1000)	//�p�[�e�B�N���ő吔
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//------------�\���̒�`------------


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureWaterAtk;		// �e�N�X�`���ւ̃|�C���^
//static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuffBillboard;		// ���_�o�b�t�@�ւ̃|�C���^
//=============================================================================
// ����������
//=============================================================================
void CWaterAtk::Init(void)
{

}

//=============================================================================
// �I������
//=============================================================================
void CWaterAtk::Uninit(void)
{

}
//=============================================================================
// �����o�֐�
//=============================================================================
CWaterAtk::CWaterAtk() {
}
// ������
CWaterAtk::CWaterAtk(D3DXVECTOR3 pos, float rot, float pow, int pat)
{
	static float colPartition1[3][5] = { { -0.6f,-0.2f,0.2f,0.6f,1.0f },{ 2.4f,2.8f,3.2f,3.6f,4.0f } ,{ 1.4f,1.8f,2.2f,2.6f,3.0f } };
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	m_pat = pat;
	for (int i = 0; i < 3; i++) {
		m_puddleFlag[i] = false;
	}
	switch (pat) {
	case 0:			//�p�^�[���O��������c�ɓ�����
	{
		if (pow <= 0) return;

		// Y����]�s��
		D3DXMATRIX *matrixY = new D3DXMATRIX;
		D3DXMatrixRotationY(matrixY, rot);

		m_delay = 0;		// ���炷���ԃ��Z�b�g
		m_numParticle = pow * MAX_WATER;		// �p�[�e�B�N����

		// �p�[�e�B�N���̓��I�m��
		m_particle = new WATER_PARTICLE[m_numParticle];

		// �擪�A�h���X�ێ�
		m_firstParticle = m_particle;
		//�z��̒����m�F���Ė��g�p�̃p�[�e�B�N�����Z�b�g����
		for (int i = 0; i < m_numParticle; i++, m_particle++)
		{
			//�K�v�Ȃ��̂��Z�b�g���Ă���
			m_particle->bUse = true;
			m_particle->delay = rand() % 10;
			m_particle->pos = pos;
			m_particle->accel = D3DXVECTOR3(0.0f, -0.2f, 0.0f);

			int angle = rand() % 360;

			float waterWidth = (rand() % 10) / 10.0f;
			float waterHeight = (rand() % 10) / 10.0f;
			m_particle->move = D3DXVECTOR3((cos(angle*D3DX_PI / 180)*waterWidth),				// X
				3.0f + (sin(angle*D3DX_PI / 180)*waterHeight),		// Y
				(rand() % 150) / 150.0f + 2.0f);					// Z

			// �����蔻��O���[�v
			for (int x = 0; x < 5; x++) {
				if (m_particle->move.x > colPartition1[0][x])	continue;
				for (int y = 0; y < 5; y++) {
					if (m_particle->move.y > colPartition1[1][y])	continue;
					for (int z = 0; z < 5; z++) {
						if (m_particle->move.z > colPartition1[2][z])	continue;

						// �o�^
						m_particle->colGroup = x * 25 + y * 5 + z;
						m_colList[m_particle->colGroup].push_back(m_particle);
						break;
					}
					break;
				}
				break;
			}

			// Y����]
			D3DXVec3TransformCoord(&m_particle->move, &m_particle->move, matrixY);
		}
		m_particle = m_firstParticle;		// �擪�A�h���X��

		// Y����]�s��폜
		delete matrixY;
		matrixY = NULL;

		break;
	}
	case 1:			//�p�^�[���P�͉��ɓ�����
	{
		if (pow <= 0) return;

		// Y����]�s��
		D3DXMATRIX *matrixY = new D3DXMATRIX;

		m_delay = 0;		// ���炷���ԃ��Z�b�g
		m_numParticle = pow * MAX_WATER;		// �p�[�e�B�N����

		// �p�[�e�B�N���̓��I�m��
		m_particle = new WATER_PARTICLE[m_numParticle];

		// �擪�A�h���X�ێ�
		m_firstParticle = m_particle;
		//�z��̒����m�F���Ė��g�p�̃p�[�e�B�N�����Z�b�g����
		for (int i = 0; i < m_numParticle; i++, m_particle++)
		{
			//�K�v�Ȃ��̂��Z�b�g���Ă���
			m_particle->bUse = true;
			m_particle->delay = rand() % 6;
			m_particle->pos = pos;
			m_particle->accel = D3DXVECTOR3(0.0f, -0.2f, 0.0f);

			D3DXMatrixRotationY(matrixY, (rot + (m_particle->delay - 3.0f) / -3.0f));			//���ԍ��ŉ�]��^����
			int angle = rand() % 360;
			float waterWidth = (rand() % 10) / 10.0f;
			float waterHeight = (rand() % 6) / 10.0f;
			m_particle->move = D3DXVECTOR3((cos(angle*D3DX_PI / 180)*waterWidth), 3.0f + (sin(angle*D3DX_PI / 180)*waterHeight), (rand() % 150) / 150.0f + 2.0f);

			// �����蔻��O���[�v
			for (int d = 0; d < 6; d++) {
				if (m_particle->delay != d) continue;
				for (int x = 0; x < 5; x++) {
					if (m_particle->move.x > colPartition1[0][x])	continue;
					for (int y = 0; y < 5; y++) {
						if (m_particle->move.y > colPartition1[1][y])	continue;
						for (int z = 0; z < 5; z++) {
							if (m_particle->move.z > colPartition1[2][z])	continue;
							// �o�^
							m_particle->colGroup = d * 125 + x * 25 + y * 5 + z;
							m_colList[m_particle->colGroup].push_back(m_particle);
							break;
						}
						break;
					}
					break;
				}
				break;
			}

			// Y����]
			D3DXVec3TransformCoord(&m_particle->move, &m_particle->move, matrixY);
		}
		m_particle = m_firstParticle;		// �擪�A�h���X��

		// Y����]�s��폜
		delete matrixY;
		matrixY = NULL;

		break;
	}
	default:
	{
		break;
	}
	}
}

CWaterAtk::~CWaterAtk()
{
	float volume;
	if (m_puddleFlag[0] && m_pat == 0) {
		volume = m_numParticle / MAX_WATER;
		CWaterPuddle::CreateWaterPuddle(m_puddlePos[0], volume, false);
	}
	else if (m_pat == 1) {
		volume = m_numParticle / MAX_WATER*0.4f;
		if (m_puddleFlag[0]) {
			for (int i = 0; i < m_numParticle; i++) {
				CWaterPuddle::CreateWaterPuddle(m_puddlePos[0], volume, false);
				break;
			}
		}
		if (m_puddleFlag[1]) {
			for (int i = 0; i < m_numParticle; i++) {
				CWaterPuddle::CreateWaterPuddle(m_puddlePos[1], volume, false);
				break;
			}
		}
		if (m_puddleFlag[2]) {
			for (int i = 0; i < m_numParticle; i++) {
				CWaterPuddle::CreateWaterPuddle(m_puddlePos[2], volume, false);
				break;
			}
		}
	}
	delete[] m_particle;
	m_colList->clear();
}
//=============================================================================
// �X�V����
// �߂�l�Ffalse�Ȃ�S�p�[�e�B�N���������Ă���
//=============================================================================
bool CWaterAtk::Update(void)
{
	m_delay++;
	cnt = 0;
	if (m_delay > 1000) {
		return false;
	}
	bool deleteFlag = false;

	// �ǂƂ̏Փ˔���
	for (int colNo = 0; colNo < WATER_MAX_COL; colNo++) {
		if (m_colList[colNo].size() == 0)	continue;
		bool ground;
		if (CollisionWaterStage(m_colList[colNo][0]->pos, m_colList[colNo][0]->move, CStage::m_stage->m_pColStageMeshModel, &m_colList[colNo][0]->pos, &ground)) {
			if (m_pat == 0 && ground&&colNo == 73) {
				m_puddlePos[0] = m_colList[colNo][0]->pos;
				m_puddleFlag[0] = true;
			}
			else {
				if (colNo == 188 && m_pat == 1 && ground) {
					m_puddlePos[0] = m_colList[colNo][0]->pos;
					m_puddleFlag[0] = true;
				}
				if (colNo == 439 && m_pat == 1 && ground) {
					m_puddlePos[1] = m_colList[colNo][0]->pos;
					m_puddleFlag[1] = true;
				}
				if (colNo == 689 && m_pat == 1 && ground) {
					m_puddlePos[2] = m_colList[colNo][0]->pos;
					m_puddleFlag[2] = true;
				}
			}
			for (int i = 0; i < m_colList[colNo].size(); i++) {
				m_colList[colNo][i]->bUse = false;
			}
			m_colList[colNo].clear();
		}
	}

	for (int i = 0; i < m_numParticle; i++, m_particle++)
	{
		//���g�p�̃p�[�e�B�N���͎g�p���Ȃ�
		if (!m_particle->bUse)
		{
			continue;
		}
		if (m_delay < m_particle->delay)	continue;

		//���x�X�V
		m_particle->move += m_particle->accel;

		//���W�X�V
		m_particle->pos += m_particle->move;

		//�p�[�e�B�N���̔j��
		if (m_particle->pos.y < 0)
		{
			m_particle->bUse = false;
			continue;
		}
		deleteFlag = true;
		particleNo[cnt] = i;
		cnt++;
	}
	m_particle = m_firstParticle;			// �擪�A�h���X��
	return deleteFlag;
}
#include "debug.h"
//=============================================================================
// �`�揈��
//=============================================================================
void CWaterAtk::Draw(int no)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	struct COLOR_VERTEX {
		D3DXVECTOR3 vecCoord;
		//	FLOAT w;
		FLOAT fScale;
		DWORD dwColor;
	};
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	// �|�C���g�X�v���C�g�̓f�B�X�v���C��Px�ŕ`�悵�Ă��܂�
	int dispx = GetSystemMetrics(SM_CXSCREEN);
	//int dispy = GetSystemMetrics(SM_CYSCREEN);
	D3DXVECTOR3 sub = CCamera::GetPosCamera(no) - m_particle[particleNo[0]].pos;
	float len = D3DXVec3Length(&sub);
	if (len > 1000) return;
	FLOAT fScale = dispx*0.5f;
	fScale /= len*0.3f;
	COLOR_VERTEX* pointSprite = new COLOR_VERTEX[cnt];
	for (int i = 0; i < cnt; ++i) {
		pointSprite[i].dwColor = D3DCOLOR_ARGB(120, 32, 32, 100);
		//	pointSprite[i].w = 1;
		pointSprite[i].fScale = fScale;
		pointSprite[i].vecCoord = m_particle[particleNo[i]].pos;
	}

	SetRenderMode(BLEND_SCREEN);
	// �[�x�o�b�t�@������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ���C�e�B���O������
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	pDevice->SetTexture(0, g_pD3DTextureWaterAtk);
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE);
	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_POINTLIST, cnt, pointSprite, sizeof(COLOR_VERTEX));
	SetRenderMode(BLEND_TRANSLUCENT);

	delete[] pointSprite;

	// ���C�e�B���O�L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void CWaterAtk::CreateBillboard()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
	//	D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
	//	FVF_VERTEX_3D,				 // �g�p���钸�_�t�H�[�}�b�g
	//	D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
	//	&g_pD3DVtxBuffBillboard,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	//	NULL);						// NULL�ɐݒ�

	//{//���_�o�b�t�@�̒��g�𖄂߂�
	//	VERTEX_3D *pVtx;

	//	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	//	g_pD3DVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//	for (int nCntVtxY = 0; nCntVtxY < 1; nCntVtxY++)
	//	{
	//		for (int nCntVtxX = 0; nCntVtxX < 2; nCntVtxX++, pVtx += 2)
	//		{
	//			// ���_���W�̐ݒ�
	//			pVtx[0].vtx.x = -(0.5f) * WATER_SIZE + nCntVtxX * WATER_SIZE;
	//			pVtx[0].vtx.y = (0.5f) * WATER_SIZE - WATER_SIZE;
	//			pVtx[0].vtx.z = 0.0f;
	//			pVtx[1].vtx.x = -(0.5f) * WATER_SIZE + nCntVtxX * WATER_SIZE;
	//			pVtx[1].vtx.y = (0.5f) * WATER_SIZE;
	//			pVtx[1].vtx.z = 0.0f;

	//			// �@���̐ݒ�
	//			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//			// ���ˌ��̐ݒ�
	//			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	//			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//			// �e�N�X�`�����W�̐ݒ�
	//			pVtx[0].tex.x = nCntVtxX;
	//			pVtx[0].tex.y = (nCntVtxY + 1);
	//			pVtx[1].tex.x = nCntVtxX;
	//			pVtx[1].tex.y = nCntVtxY;
	//		}
	//	}

	//	// ���_�f�[�^���A�����b�N����
	//	g_pD3DVtxBuffBillboard->Unlock();
	//}

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_WATER,			// �t�@�C���̖��O
		&g_pD3DTextureWaterAtk);	// �ǂݍ��ރ������[
}
void CWaterAtk::ReleaseBillboard() {
	//if (g_pD3DTextureWaterAtk != NULL)
	//{
	//	//���_���̉��
	//	g_pD3DTextureWaterAtk->Release();
	//	g_pD3DTextureWaterAtk = NULL;
	//}
	//SAFE_RELEASE(g_pD3DVtxBuffBillboard);
}

// �p�[�e�B�N���̈ʒu���擾
D3DXVECTOR3 CWaterAtk::GetPos(int no, int *num) {
	if (m_colList[no].size() == 0) {
		*num = 0;
		return D3DXVECTOR3(9999, 9999, 9999);
	}
	*num = m_colList[no].size();
	return m_colList[no][0]->pos;
}


//�p�[�e�B�N���̐����擾
int CWaterAtk::GetNumParticle()
{
	return m_numParticle;
}

// �p�[�e�B�N���̃O���[�v�폜
void CWaterAtk::SetUseDown(int no) {
	for (int i = 0; i < m_colList[no].size(); i++) {
		m_colList[no][i]->bUse = false;
	}
	m_colList[no].clear();
}