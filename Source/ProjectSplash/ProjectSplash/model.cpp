//=============================================================================
//
// ���f������ [model.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include <stdlib.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// X�t�@�C���ǂݍ���
//=============================================================================
HRESULT LoadModel(LPCTSTR pszFName, MODEL* pModel)
{
	TCHAR				szDir[_MAX_DIR]="data/MODEL";
	TCHAR				szCurrentDir[_MAX_PATH];
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	LPD3DXBUFFER		pBuffMaterial = NULL;

	if (!pModel) return E_FAIL;

	// �t�H���_���𒊏o
	//_tsplitpath(pszFName, NULL, szDir, NULL, NULL);

	// X�t�@�C���ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(pszFName,
		D3DXMESH_SYSTEMMEM, pDevice, NULL, &pBuffMaterial,
		NULL, &pModel->dwNumMat, &pModel->pMesh);
	if (FAILED(hr)) return hr;

	// �����e�[�u���擾
	pModel->pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	pModel->dwAttr = 0;
	pModel->pMesh->GetAttributeTable(NULL, &pModel->dwAttr);
	pModel->pAttr = new D3DXATTRIBUTERANGE[pModel->dwAttr];
	pModel->pMesh->GetAttributeTable(pModel->pAttr, &pModel->dwAttr);

	// �J�����g�f�B���N�g����ύX
	if (szDir[0]) {
		GetCurrentDirectory(_MAX_PATH, szCurrentDir);
		SetCurrentDirectory(szDir);
	}

	// �}�e���A���ǂݍ���
	D3DXMATERIAL* pMaterial = (D3DXMATERIAL*)pBuffMaterial->GetBufferPointer();
	pModel->pMaterial = new D3DMATERIAL9[pModel->dwNumMat];
	pModel->ppTexture = new LPDIRECT3DTEXTURE9[pModel->dwNumMat];
	for (DWORD i = 0; i < pModel->dwNumMat; ++i) {
		pModel->pMaterial[i] = pMaterial[i].MatD3D;
		pModel->pMaterial[i].Ambient = pModel->pMaterial[i].Diffuse;
		pModel->ppTexture[i] = NULL;
		if (pMaterial[i].pTextureFilename && pMaterial[i].pTextureFilename[0]) {
			// �e�N�X�`���t�@�C����ǂݍ���
			hr = D3DXCreateTextureFromFileA(pDevice,
				pMaterial[i].pTextureFilename, &pModel->ppTexture[i]);
			if (FAILED(hr)) {
				pModel->ppTexture[i] = NULL;
				SAFE_RELEASE(pBuffMaterial);
				return hr;
			}
		}
	}
	SAFE_RELEASE(pBuffMaterial);

	// �J�����g�f�B���N�g�������ɖ߂�
	if (szDir[0])
		SetCurrentDirectory(szCurrentDir);

	return hr;
}

//=============================================================================
// X�t�@�C���\��
//=============================================================================
void DrawModel(MODEL* pModel)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	if (!pModel) return;
	for (DWORD i = 0; i < pModel->dwAttr; ++i) {
		DWORD id = pModel->pAttr[i].AttribId;
		pDevice->SetMaterial(&pModel->pMaterial[id]);	// �}�e���A����ݒ�
		pDevice->SetTexture(0, pModel->ppTexture[id]);	// �e�N�X�`����ݒ�
		pModel->pMesh->DrawSubset(i);					// �`������s
	}
}

//=============================================================================
// X�t�@�C���s���������\��
//=============================================================================
void DrawModelNoAlpha(MODEL* pModel)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	if (!pModel) return;
	for (DWORD i = 0; i < pModel->dwAttr; ++i) {
		DWORD id = pModel->pAttr[i].AttribId;
		if (pModel->pMaterial[id].Diffuse.a < 1.0f) continue;
		pDevice->SetMaterial(&pModel->pMaterial[id]);	// �}�e���A����ݒ�
		pDevice->SetTexture(0, pModel->ppTexture[id]);	// �e�N�X�`����ݒ�
		pModel->pMesh->DrawSubset(i);					// �`������s
	}
}

//=============================================================================
// X�t�@�C���������\��
//=============================================================================
void DrawModelAlpha(MODEL* pModel, float fAlpha)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DMATERIAL9 material;

	if (!pModel) return;
	for (DWORD i = 0; i < pModel->dwAttr; ++i) {
		DWORD id = pModel->pAttr[i].AttribId;
		material = pModel->pMaterial[id];
		material.Diffuse.a *= fAlpha;
		if (material.Diffuse.a >= 1.0f) continue;
		pDevice->SetMaterial(&material);	// �}�e���A����ݒ�
		pDevice->SetTexture(0, pModel->ppTexture[id]);	// �e�N�X�`����ݒ�
		pModel->pMesh->DrawSubset(i);					// �`������s
	}
}

//=============================================================================
// X�t�@�C�����
//=============================================================================
void ReleaseModel(MODEL* pModel)
{
	if (!pModel) return;
	for (DWORD i = 0; i < pModel->dwNumMat; ++i) {
		SAFE_RELEASE(pModel->ppTexture[i]);
	}
	pModel->dwNumMat = 0;
	SAFE_DELETE_ARRAY(pModel->ppTexture);
	SAFE_DELETE_ARRAY(pModel->pMaterial);
	SAFE_RELEASE(pModel->pMesh);
}
