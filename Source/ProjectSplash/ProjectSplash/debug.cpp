//////////////////////////////////////////
//
// debug.cpp
// デバッグに使えそうなもの
//
/////////////////////////////////////////
//--------------インクルード----------------
#include "main.h"
#include "model.h"
//--------------グローバル変数----------------
static LPDIRECT3DDEVICE9	g_pDevice;
static ID3DXLine			*g_pLine;
static LPD3DXFONT			g_pD3DXFont = NULL;		// フォントへのポインタ
static MODEL				g_box;
//---------------関数---------------
// 初期化
HRESULT InitDebug() {
	HRESULT hr;
	// デバイス取得
	g_pDevice = GetDevice();
	// ラインオブジェクト生成
	hr = D3DXCreateLine(g_pDevice, &g_pLine);
	if (FAILED(hr)) {
		return hr;
	}
	hr = LoadModel("data/MODEL/box.x", &g_box);
	if (FAILED(hr)) {
		return hr;
	}
	return hr;
}

/*
線を引きます
引数：ラインの頂点
*/
void DrawDebugLine(D3DXVECTOR2 vertex1, D3DXVECTOR2 vertex2) {
	// 頂点作成
	D3DXVECTOR2 *vec;
	vec = new D3DXVECTOR2[2];
	vec[0] = vertex1;
	vec[1] = vertex2;

	// 線描画
	g_pLine->Begin();
	g_pLine->Draw(vec, 2, D3DCOLOR_RGBA(255, 0, 0, 255));
	g_pLine->End();

	// 解放
	if (vec != NULL) {
		delete[] vec;
		vec = NULL;
	}
}

/*
ボックス書きます
引数：位置,大きさ
*/
void DrawDebugBox(D3DXVECTOR3 pos, D3DXVECTOR3 size) {
	// 頂点作成
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxSize,mtxWorld;
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxSize, size.x,size.y,size.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxSize);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->BeginScene();
	//描画
	DrawModel(&g_box);	

}