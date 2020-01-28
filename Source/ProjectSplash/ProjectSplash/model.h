//=============================================================================
//
// ���f������ [model.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#ifndef ___MODEL_H_
#define ___MODEL_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct _MODEL {
	LPD3DXMESH			pMesh;		// ���b�V��
	D3DMATERIAL9*		pMaterial;	// �}�e���A���z��
	DWORD				dwNumMat;	// �}�e���A����
	LPDIRECT3DTEXTURE9*	ppTexture;	// �e�N�X�`���z��
	DWORD				dwAttr;		// ������(�T�u�Z�b�g��)
	D3DXATTRIBUTERANGE*	pAttr;		// �����z��
} MODEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT LoadModel(LPCTSTR pszFName, MODEL* pModel);
void DrawModel(MODEL* pModel);
void DrawModelNoAlpha(MODEL* pModel);
void DrawModelAlpha(MODEL* pModel, float fAlpha);
void ReleaseModel(MODEL* pModel);

#endif
