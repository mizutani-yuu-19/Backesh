//=============================================================================
//
// モデル処理 [model.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#ifndef ___MODEL_H_
#define ___MODEL_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct _MODEL {
	LPD3DXMESH			pMesh;		// メッシュ
	D3DMATERIAL9*		pMaterial;	// マテリアル配列
	DWORD				dwNumMat;	// マテリアル数
	LPDIRECT3DTEXTURE9*	ppTexture;	// テクスチャ配列
	DWORD				dwAttr;		// 属性数(サブセット数)
	D3DXATTRIBUTERANGE*	pAttr;		// 属性配列
} MODEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT LoadModel(LPCTSTR pszFName, MODEL* pModel);
void DrawModel(MODEL* pModel);
void DrawModelNoAlpha(MODEL* pModel);
void DrawModelAlpha(MODEL* pModel, float fAlpha);
void ReleaseModel(MODEL* pModel);

#endif
