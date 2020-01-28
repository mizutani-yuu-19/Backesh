//=============================================================================
//
// �����܂菈�� [water_puddle.cpp]
// 
//
//=============================================================================
#include "input.h"
#include "water_puddle.h"
#include <tchar.h>
#include <stdlib.h>
#include "collision.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_PUDDLE			"data/TEXTURE/water_puddle.png"


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//------------�\���̒�`------------

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
std::vector<CWaterPuddle*>	CWaterPuddle::m_waterPuddle;				// ����

static LPDIRECT3DTEXTURE9		g_pD3DTexturePuddle;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBoard;		// ���_�o�b�t�@�ւ̃|�C���^

int CWaterPuddle::NoCnt = 0;
//=============================================================================
// ����������
//=============================================================================
void CWaterPuddle::Init(void)
{

}
//=============================================================================
// �I������
//=============================================================================
void CWaterPuddle::Uninit(void)
{

}

//=============================================================================
// �����o�֐�
//=============================================================================
// �R���X�g���N�^
CWaterPuddle::CWaterPuddle(D3DXVECTOR3 pos, float vol, bool infinite)
{
	m_pos = pos;
	m_infiniteFlag = infinite;
	//m_size = size;
	m_remain = vol;
	m_size = D3DXVECTOR3(vol*100.0f, 1, vol*100.0f);
	if (infinite)	m_remain = 50.0f;
	for (int i = m_waterPuddle.size() - 1; i >= 0; i--) {
		// ���Ƃ��ɋ߂���΂ق��̐��Ɠ���
		if (CollisionAABB(m_pos, m_size, GetWaterPuddlePos(i), GetWaterPuddleSize(i))) {
			if (m_remain > m_waterPuddle[i]->m_remain) {
				m_remain += m_waterPuddle[i]->m_remain;
				delete m_waterPuddle[i];
				m_waterPuddle[i] = NULL;
				m_waterPuddle.erase(m_waterPuddle.begin() + i);
			}
			else {
				m_waterPuddle[i]->m_remain += m_remain;
			}
			break;
		}
	}
	m_handle = NoCnt++;
}

CWaterPuddle::~CWaterPuddle()
{

}
//=============================================================================
// �X�V����
//=============================================================================
void CWaterPuddle::Update(void)
{
	if (m_infiniteFlag)
		return;
	m_remain -= 0.001f;
	m_size = D3DXVECTOR3(1, 0.01f, 1)*m_remain*10.0f;
	m_size.y = 1;
	if (m_remain <= 0.0f) {
		for (int i = 0; i < m_waterPuddle.size(); i++) {
			if (m_waterPuddle[i] == this) {
				delete this;
				m_waterPuddle[i] = NULL;
				m_waterPuddle.erase(m_waterPuddle.begin() + i);
				break;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWaterPuddle::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxScl, mtxRot, mtxTrans;
	D3DXMATERIAL*		pD3DXMat;
	D3DMATERIAL9		matDef;

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, g_pD3DVtxBoard, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTexturePuddle);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_size.x, 1, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//�����܂�̍��W���擾
D3DXVECTOR3 CWaterPuddle::GetWaterPuddlePos(int no)
{
	return m_waterPuddle[no]->m_pos;
}

//�����܂�̑傫�����擾
D3DXVECTOR3 CWaterPuddle::GetWaterPuddleSize(int no)
{
	return m_waterPuddle[no]->m_size*PUDDLE_SIZE;
}

//�����܂�̃n���h���擾
int CWaterPuddle::GetWaterHandle(int no)
{
	return m_waterPuddle[no]->m_handle;
}

//�����܂�̐��ʂ��擾
float CWaterPuddle::GetWaterPuddleVolume(int no)
{
	if (m_waterPuddle[no]->m_infiniteFlag)	return 50.0f;
	return m_waterPuddle[no]->m_remain;
}

// �����܂���z��static
void CWaterPuddle::SubRemain(int no) {
	m_waterPuddle[no]->Absorb();
}

// �����܂���z��static
void CWaterPuddle::SubRemain2(int handle, float volume) {
	for (int i = m_waterPuddle.size() - 1; i >= 0; i--) {
		if (m_waterPuddle[i]->m_handle == handle) {
			m_waterPuddle[i]->Absorb(volume);
			return;
		}
	}
}

// �����܂���z��
void CWaterPuddle::Absorb() {
	if (m_infiniteFlag)	return;
	m_remain -= 0.01f;
}
// �����܂���z��
void CWaterPuddle::Absorb(float vol) {
	if (m_infiniteFlag)	return;
	m_remain -= vol;
}

void CWaterPuddle::LoadModel() {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				 // �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBoard,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);						// NULL�ɐݒ�

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBoard->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxY = 0; nCntVtxY < 1; nCntVtxY++)
		{
			for (int nCntVtxX = 0; nCntVtxX < 2; nCntVtxX++, pVtx += 2)
			{
				// ���_���W�̐ݒ�
				pVtx[0].vtx.x = -(0.5f) * PUDDLE_SIZE + nCntVtxX * PUDDLE_SIZE;
				pVtx[0].vtx.y = 0.1f;
				pVtx[0].vtx.z = (0.5f) * PUDDLE_SIZE - PUDDLE_SIZE;
				pVtx[1].vtx.x = -(0.5f) * PUDDLE_SIZE + nCntVtxX * PUDDLE_SIZE;
				pVtx[1].vtx.y = 0.1f;
				pVtx[1].vtx.z = (0.5f) * PUDDLE_SIZE;

				// �@���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex.x = nCntVtxX;
				pVtx[0].tex.y = (nCntVtxY + 1);
				pVtx[1].tex.x = nCntVtxX;
				pVtx[1].tex.y = nCntVtxY;
			}
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBoard->Unlock();
	}

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PUDDLE,			// �t�@�C���̖��O
		&g_pD3DTexturePuddle);	// �ǂݍ��ރ������[
	NoCnt = 0;
}
void CWaterPuddle::ReleaseModel() {
	SAFE_RELEASE(g_pD3DVtxBoard);
	SAFE_RELEASE(g_pD3DTexturePuddle);
	for (int i = 0; i < m_waterPuddle.size(); i++) {
		m_waterPuddle[i]->Uninit();
		SAFE_DELETE(m_waterPuddle[i]);
	}
	m_waterPuddle.clear();
}

void CWaterPuddle::CreateWaterPuddle(D3DXVECTOR3 pos, float vol, bool infinite) {
	CWaterPuddle *temp = new CWaterPuddle(pos, vol, infinite);
	m_waterPuddle.push_back(temp);
	temp = NULL;
}

void CWaterPuddle::UpdateWaterPuddle() {
	// �߂���΂ق��̐��Ɠ��� ��������ŏd������̂ł��
	for (int i = m_waterPuddle.size() - 1; i >= 0; i--) {
		for (int j = i - 1; j >= 0; j--) {
			//if (i == j)	continue;
			if (CollisionAABB(GetWaterPuddlePos(j), GetWaterPuddleSize(j), GetWaterPuddlePos(i), GetWaterPuddleSize(i))) {
				if (GetWaterPuddleVolume(j) > GetWaterPuddleVolume(i)) {
					// �傫���Ȃ�߂��Ȃ��悤��
					if (m_waterPuddle[j]->m_remain < 5.0f) {
						m_waterPuddle[j]->m_remain += 0.01f;
					}
					if (m_waterPuddle[i]->m_remain < 0.1f) {
						m_waterPuddle[i]->m_remain -= 0.1f;
					}
					else {
						m_waterPuddle[i]->m_remain -= 0.01f;
					}
					if (GetWaterPuddleVolume(i) <= 0.0f) {
						delete m_waterPuddle[i];
						m_waterPuddle[i] = NULL;
						m_waterPuddle.erase(m_waterPuddle.begin() + i);
					}
				}
				else {
					// �傫���Ȃ�߂��Ȃ��悤��
					if (m_waterPuddle[i]->m_remain < 5.0f) {
						m_waterPuddle[i]->m_remain += 0.01f;
					}
					if (m_waterPuddle[j]->m_remain < 0.1f) {
						m_waterPuddle[j]->m_remain -= 0.1f;
					}
					else {
						m_waterPuddle[j]->m_remain -= 0.01f;
					}
					if (GetWaterPuddleVolume(j) <= 0.0f) {
						delete m_waterPuddle[j];
						m_waterPuddle[j] = NULL;
						m_waterPuddle.erase(m_waterPuddle.begin() + j);
					}
				}
				break;
			}
		}
	}
	for (int i = 0; i < m_waterPuddle.size(); i++) {
		m_waterPuddle[i]->Update();
	}
}

void CWaterPuddle::DrawWaterPuddle() {
	for (int i = 0; i < m_waterPuddle.size(); i++) {
		m_waterPuddle[i]->Draw();
	}
}