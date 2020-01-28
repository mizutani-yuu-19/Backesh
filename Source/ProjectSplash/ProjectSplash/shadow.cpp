//=============================================================================
//
// �ۉe�̏��� [shadow.cpp]
//
//=============================================================================
#include "shadow.h"
#include "character.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9 g_pD3DTextureShadow;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffShadow;// ���_�o�b�t�@�ւ̃|�C���^

static D3DXMATRIX g_mtxWorldShadow;				// ���[���h�}�g���b�N�X
static D3DXVECTOR3 g_posShadow;					// �ʒu
static D3DXVECTOR3 g_rotShadow;					// ����

static int g_nNumBlockX, g_nNumBlockZ;				// �u���b�N��
static int g_nNumVertex;							// �����_��
static int g_nNumPolygon;							// ���|���S����
static float g_fSizeBlockX, g_fSizeBlockZ;			// �u���b�N�T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// �ʒu�E�����̏����ݒ�
	g_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FILENAME,		// �t�@�C���̖��O
								&g_pD3DTextureShadow);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	hr = MakeVertexShadow(pDevice);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	if(g_pD3DVtxBuffShadow)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffShadow->Release();
		g_pD3DVtxBuffShadow = NULL;
	}

	if(g_pD3DTextureShadow)
	{// �e�N�X�`���̊J��
		g_pD3DTextureShadow->Release();
		g_pD3DTextureShadow = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{
	D3DXVECTOR3 pos = CCharacter::GetPlayerPos(0);
	g_posShadow.x = pos.x;
	g_posShadow.z = pos.z;
}

//=============================================================================
// �`�揉��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldShadow);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotShadow.y, g_rotShadow.x, g_rotShadow.z);
	D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posShadow.x, g_posShadow.y, g_posShadow.z);
	D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, g_pD3DVtxBuffShadow, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureShadow);

	// ���Z�����̐ݒ�
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�o�b�t�@���Q�Ƃ��Ȃ�
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, g_nNumPolygon);

	// Z�o�b�t�@���Q�Ƃ���
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ���Z����������
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// �u���b�N��
	g_nNumBlockX = 1;
	g_nNumBlockZ = 1;

	// ���_��
	g_nNumVertex = 4;

	// �|���S����
	g_nNumPolygon = 2;

	// �u���b�N�T�C�Y
	g_fSizeBlockX = 20.0f;
	g_fSizeBlockZ = 20.0f;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertex,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffShadow,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		const float fSizeTexX = 1.0f;
		const float fSizeTexZ = 1.0f;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffShadow->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int nCntVtxZ = 0; nCntVtxZ < g_nNumBlockZ; nCntVtxZ++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockX + 1); nCntVtxX++, pVtx += 2)
			{
				// ���_���W�̐ݒ�
				pVtx[0].vtx.x = -(g_nNumBlockX / 2.0f) * g_fSizeBlockX + nCntVtxX * g_fSizeBlockX;
				pVtx[0].vtx.y = 0.0f;
				pVtx[0].vtx.z = (g_nNumBlockZ / 2.0f) * g_fSizeBlockZ - (nCntVtxZ + 1) * g_fSizeBlockZ;
				pVtx[1].vtx.x = -(g_nNumBlockX / 2.0f) * g_fSizeBlockX + nCntVtxX * g_fSizeBlockX;
				pVtx[1].vtx.y = 0.0f;
				pVtx[1].vtx.z = (g_nNumBlockZ / 2.0f) * g_fSizeBlockZ - nCntVtxZ * g_fSizeBlockZ;

				// �@���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex.x = fSizeTexX * nCntVtxX;
				pVtx[0].tex.y = fSizeTexZ * (nCntVtxZ + 1);
				pVtx[1].tex.x = fSizeTexX * nCntVtxX;
				pVtx[1].tex.y = fSizeTexZ * nCntVtxZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffShadow->Unlock();
	}

	return S_OK;
}
