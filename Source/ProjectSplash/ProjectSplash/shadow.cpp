//=============================================================================
//
// 丸影の処理 [shadow.cpp]
//
//=============================================================================
#include "shadow.h"
#include "character.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME	"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9 g_pD3DTextureShadow;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffShadow;// 頂点バッファへのポインタ

static D3DXMATRIX g_mtxWorldShadow;				// ワールドマトリックス
static D3DXVECTOR3 g_posShadow;					// 位置
static D3DXVECTOR3 g_rotShadow;					// 向き

static int g_nNumBlockX, g_nNumBlockZ;				// ブロック数
static int g_nNumVertex;							// 総頂点数
static int g_nNumPolygon;							// 総ポリゴン数
static float g_fSizeBlockX, g_fSizeBlockZ;			// ブロックサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// 位置・向きの初期設定
	g_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&g_pD3DTextureShadow);	// 読み込むメモリー

	// 頂点情報の作成
	hr = MakeVertexShadow(pDevice);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	if(g_pD3DVtxBuffShadow)
	{// 頂点バッファの開放
		g_pD3DVtxBuffShadow->Release();
		g_pD3DVtxBuffShadow = NULL;
	}

	if(g_pD3DTextureShadow)
	{// テクスチャの開放
		g_pD3DTextureShadow->Release();
		g_pD3DTextureShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{
	D3DXVECTOR3 pos = CCharacter::GetPlayerPos(0);
	g_posShadow.x = pos.x;
	g_posShadow.z = pos.z;
}

//=============================================================================
// 描画初理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldShadow);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotShadow.y, g_rotShadow.x, g_rotShadow.z);
	D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posShadow.x, g_posShadow.y, g_posShadow.z);
	D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, g_pD3DVtxBuffShadow, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureShadow);

	// 減算合成の設定
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Zバッファを参照しない
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, g_nNumPolygon);

	// Zバッファを参照する
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// 減算合成を解除
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// ブロック数
	g_nNumBlockX = 1;
	g_nNumBlockZ = 1;

	// 頂点数
	g_nNumVertex = 4;

	// ポリゴン数
	g_nNumPolygon = 2;

	// ブロックサイズ
	g_fSizeBlockX = 20.0f;
	g_fSizeBlockZ = 20.0f;

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_3D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffShadow,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
		const float fSizeTexX = 1.0f;
		const float fSizeTexZ = 1.0f;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffShadow->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int nCntVtxZ = 0; nCntVtxZ < g_nNumBlockZ; nCntVtxZ++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockX + 1); nCntVtxX++, pVtx += 2)
			{
				// 頂点座標の設定
				pVtx[0].vtx.x = -(g_nNumBlockX / 2.0f) * g_fSizeBlockX + nCntVtxX * g_fSizeBlockX;
				pVtx[0].vtx.y = 0.0f;
				pVtx[0].vtx.z = (g_nNumBlockZ / 2.0f) * g_fSizeBlockZ - (nCntVtxZ + 1) * g_fSizeBlockZ;
				pVtx[1].vtx.x = -(g_nNumBlockX / 2.0f) * g_fSizeBlockX + nCntVtxX * g_fSizeBlockX;
				pVtx[1].vtx.y = 0.0f;
				pVtx[1].vtx.z = (g_nNumBlockZ / 2.0f) * g_fSizeBlockZ - nCntVtxZ * g_fSizeBlockZ;

				// 法線の設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex.x = fSizeTexX * nCntVtxX;
				pVtx[0].tex.y = fSizeTexZ * (nCntVtxZ + 1);
				pVtx[1].tex.x = fSizeTexX * nCntVtxX;
				pVtx[1].tex.y = fSizeTexZ * nCntVtxZ;
			}
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffShadow->Unlock();
	}

	return S_OK;
}
