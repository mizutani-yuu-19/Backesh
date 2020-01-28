//=============================================================================
//
// 水たまり処理 [water_puddle.cpp]
// 
//
//=============================================================================
#include "input.h"
#include "water_puddle.h"
#include <tchar.h>
#include <stdlib.h>
#include "collision.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_PUDDLE			"data/TEXTURE/water_puddle.png"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//------------構造体定義------------

//*****************************************************************************
// グローバル変数
//*****************************************************************************
std::vector<CWaterPuddle*>	CWaterPuddle::m_waterPuddle;				// 実体

static LPDIRECT3DTEXTURE9		g_pD3DTexturePuddle;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBoard;		// 頂点バッファへのポインタ

int CWaterPuddle::NoCnt = 0;
//=============================================================================
// 初期化処理
//=============================================================================
void CWaterPuddle::Init(void)
{

}
//=============================================================================
// 終了処理
//=============================================================================
void CWaterPuddle::Uninit(void)
{

}

//=============================================================================
// メンバ関数
//=============================================================================
// コンストラクタ
CWaterPuddle::CWaterPuddle(D3DXVECTOR3 pos, float vol, bool infinite)
{
	m_pos = pos;
	m_infiniteFlag = infinite;
	//m_size = size;
	m_remain = vol;
	m_size = D3DXVECTOR3(vol*100.0f, 1, vol*100.0f);
	if (infinite)	m_remain = 50.0f;
	for (int i = m_waterPuddle.size() - 1; i >= 0; i--) {
		// 作るときに近ければほかの水と統合
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
// 更新処理
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
// 描画処理
//=============================================================================
void CWaterPuddle::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	D3DXMATRIX			mtxScl, mtxRot, mtxTrans;
	D3DXMATERIAL*		pD3DXMat;
	D3DMATERIAL9		matDef;

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, g_pD3DVtxBoard, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTexturePuddle);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//スケールを反映
	D3DXMatrixScaling(&mtxScl, m_size.x, 1, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//水たまりの座標を取得
D3DXVECTOR3 CWaterPuddle::GetWaterPuddlePos(int no)
{
	return m_waterPuddle[no]->m_pos;
}

//水たまりの大きさを取得
D3DXVECTOR3 CWaterPuddle::GetWaterPuddleSize(int no)
{
	return m_waterPuddle[no]->m_size*PUDDLE_SIZE;
}

//水たまりのハンドル取得
int CWaterPuddle::GetWaterHandle(int no)
{
	return m_waterPuddle[no]->m_handle;
}

//水たまりの水量を取得
float CWaterPuddle::GetWaterPuddleVolume(int no)
{
	if (m_waterPuddle[no]->m_infiniteFlag)	return 50.0f;
	return m_waterPuddle[no]->m_remain;
}

// 水たまりを吸うstatic
void CWaterPuddle::SubRemain(int no) {
	m_waterPuddle[no]->Absorb();
}

// 水たまりを吸うstatic
void CWaterPuddle::SubRemain2(int handle, float volume) {
	for (int i = m_waterPuddle.size() - 1; i >= 0; i--) {
		if (m_waterPuddle[i]->m_handle == handle) {
			m_waterPuddle[i]->Absorb(volume);
			return;
		}
	}
}

// 水たまりを吸う
void CWaterPuddle::Absorb() {
	if (m_infiniteFlag)	return;
	m_remain -= 0.01f;
}
// 水たまりを吸う
void CWaterPuddle::Absorb(float vol) {
	if (m_infiniteFlag)	return;
	m_remain -= vol;
}

void CWaterPuddle::LoadModel() {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				 // 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBoard,	// 頂点バッファインターフェースへのポインタ
		NULL);						// NULLに設定

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBoard->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxY = 0; nCntVtxY < 1; nCntVtxY++)
		{
			for (int nCntVtxX = 0; nCntVtxX < 2; nCntVtxX++, pVtx += 2)
			{
				// 頂点座標の設定
				pVtx[0].vtx.x = -(0.5f) * PUDDLE_SIZE + nCntVtxX * PUDDLE_SIZE;
				pVtx[0].vtx.y = 0.1f;
				pVtx[0].vtx.z = (0.5f) * PUDDLE_SIZE - PUDDLE_SIZE;
				pVtx[1].vtx.x = -(0.5f) * PUDDLE_SIZE + nCntVtxX * PUDDLE_SIZE;
				pVtx[1].vtx.y = 0.1f;
				pVtx[1].vtx.z = (0.5f) * PUDDLE_SIZE;

				// 法線の設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex.x = nCntVtxX;
				pVtx[0].tex.y = (nCntVtxY + 1);
				pVtx[1].tex.x = nCntVtxX;
				pVtx[1].tex.y = nCntVtxY;
			}
		}

		// 頂点データをアンロックする
		g_pD3DVtxBoard->Unlock();
	}

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_PUDDLE,			// ファイルの名前
		&g_pD3DTexturePuddle);	// 読み込むメモリー
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
	// 近ければほかの水と統合 総当たりで重すぎるのでやめ
	for (int i = m_waterPuddle.size() - 1; i >= 0; i--) {
		for (int j = i - 1; j >= 0; j--) {
			//if (i == j)	continue;
			if (CollisionAABB(GetWaterPuddlePos(j), GetWaterPuddleSize(j), GetWaterPuddlePos(i), GetWaterPuddleSize(i))) {
				if (GetWaterPuddleVolume(j) > GetWaterPuddleVolume(i)) {
					// 大きくなり過ぎないように
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
					// 大きくなり過ぎないように
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