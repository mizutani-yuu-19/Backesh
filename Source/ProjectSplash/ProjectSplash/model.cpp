//=============================================================================
//
// モデル処理 [model.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include <stdlib.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// Xファイル読み込み
//=============================================================================
HRESULT LoadModel(LPCTSTR pszFName, MODEL* pModel)
{
	TCHAR				szDir[_MAX_DIR]="data/MODEL";
	TCHAR				szCurrentDir[_MAX_PATH];
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	LPD3DXBUFFER		pBuffMaterial = NULL;

	if (!pModel) return E_FAIL;

	// フォルダ名を抽出
	//_tsplitpath(pszFName, NULL, szDir, NULL, NULL);

	// Xファイル読み込み
	HRESULT hr = D3DXLoadMeshFromX(pszFName,
		D3DXMESH_SYSTEMMEM, pDevice, NULL, &pBuffMaterial,
		NULL, &pModel->dwNumMat, &pModel->pMesh);
	if (FAILED(hr)) return hr;

	// 属性テーブル取得
	pModel->pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	pModel->dwAttr = 0;
	pModel->pMesh->GetAttributeTable(NULL, &pModel->dwAttr);
	pModel->pAttr = new D3DXATTRIBUTERANGE[pModel->dwAttr];
	pModel->pMesh->GetAttributeTable(pModel->pAttr, &pModel->dwAttr);

	// カレントディレクトリを変更
	if (szDir[0]) {
		GetCurrentDirectory(_MAX_PATH, szCurrentDir);
		SetCurrentDirectory(szDir);
	}

	// マテリアル読み込み
	D3DXMATERIAL* pMaterial = (D3DXMATERIAL*)pBuffMaterial->GetBufferPointer();
	pModel->pMaterial = new D3DMATERIAL9[pModel->dwNumMat];
	pModel->ppTexture = new LPDIRECT3DTEXTURE9[pModel->dwNumMat];
	for (DWORD i = 0; i < pModel->dwNumMat; ++i) {
		pModel->pMaterial[i] = pMaterial[i].MatD3D;
		pModel->pMaterial[i].Ambient = pModel->pMaterial[i].Diffuse;
		pModel->ppTexture[i] = NULL;
		if (pMaterial[i].pTextureFilename && pMaterial[i].pTextureFilename[0]) {
			// テクスチャファイルを読み込む
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

	// カレントディレクトリを元に戻す
	if (szDir[0])
		SetCurrentDirectory(szCurrentDir);

	return hr;
}

//=============================================================================
// Xファイル表示
//=============================================================================
void DrawModel(MODEL* pModel)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	if (!pModel) return;
	for (DWORD i = 0; i < pModel->dwAttr; ++i) {
		DWORD id = pModel->pAttr[i].AttribId;
		pDevice->SetMaterial(&pModel->pMaterial[id]);	// マテリアルを設定
		pDevice->SetTexture(0, pModel->ppTexture[id]);	// テクスチャを設定
		pModel->pMesh->DrawSubset(i);					// 描画を実行
	}
}

//=============================================================================
// Xファイル不透明部分表示
//=============================================================================
void DrawModelNoAlpha(MODEL* pModel)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	if (!pModel) return;
	for (DWORD i = 0; i < pModel->dwAttr; ++i) {
		DWORD id = pModel->pAttr[i].AttribId;
		if (pModel->pMaterial[id].Diffuse.a < 1.0f) continue;
		pDevice->SetMaterial(&pModel->pMaterial[id]);	// マテリアルを設定
		pDevice->SetTexture(0, pModel->ppTexture[id]);	// テクスチャを設定
		pModel->pMesh->DrawSubset(i);					// 描画を実行
	}
}

//=============================================================================
// Xファイル半透明表示
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
		pDevice->SetMaterial(&material);	// マテリアルを設定
		pDevice->SetTexture(0, pModel->ppTexture[id]);	// テクスチャを設定
		pModel->pMesh->DrawSubset(i);					// 描画を実行
	}
}

//=============================================================================
// Xファイル解放
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
