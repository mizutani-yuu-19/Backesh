//=============================================================================
//
// モデル処理 [water_atk.cpp]
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_WATER			"data/texture/water.png"

#define VALUE_ROTATE		(D3DX_PI * 0.02f)

#define MAX_WATER			(1000)	//パーティクル最大数
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//------------構造体定義------------


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureWaterAtk;		// テクスチャへのポインタ
//static LPDIRECT3DVERTEXBUFFER9	g_pD3DVtxBuffBillboard;		// 頂点バッファへのポインタ
//=============================================================================
// 初期化処理
//=============================================================================
void CWaterAtk::Init(void)
{

}

//=============================================================================
// 終了処理
//=============================================================================
void CWaterAtk::Uninit(void)
{

}
//=============================================================================
// メンバ関数
//=============================================================================
CWaterAtk::CWaterAtk() {
}
// 水発射
CWaterAtk::CWaterAtk(D3DXVECTOR3 pos, float rot, float pow, int pat)
{
	static float colPartition1[3][5] = { { -0.6f,-0.2f,0.2f,0.6f,1.0f },{ 2.4f,2.8f,3.2f,3.6f,4.0f } ,{ 1.4f,1.8f,2.2f,2.6f,3.0f } };
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	m_pat = pat;
	for (int i = 0; i < 3; i++) {
		m_puddleFlag[i] = false;
	}
	switch (pat) {
	case 0:			//パターン０だったら縦に投げる
	{
		if (pow <= 0) return;

		// Y軸回転行列
		D3DXMATRIX *matrixY = new D3DXMATRIX;
		D3DXMatrixRotationY(matrixY, rot);

		m_delay = 0;		// ずらす時間リセット
		m_numParticle = pow * MAX_WATER;		// パーティクル数

		// パーティクルの動的確保
		m_particle = new WATER_PARTICLE[m_numParticle];

		// 先頭アドレス保持
		m_firstParticle = m_particle;
		//配列の中を確認して未使用のパーティクルをセットする
		for (int i = 0; i < m_numParticle; i++, m_particle++)
		{
			//必要なものをセットしていく
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

			// 当たり判定グループ
			for (int x = 0; x < 5; x++) {
				if (m_particle->move.x > colPartition1[0][x])	continue;
				for (int y = 0; y < 5; y++) {
					if (m_particle->move.y > colPartition1[1][y])	continue;
					for (int z = 0; z < 5; z++) {
						if (m_particle->move.z > colPartition1[2][z])	continue;

						// 登録
						m_particle->colGroup = x * 25 + y * 5 + z;
						m_colList[m_particle->colGroup].push_back(m_particle);
						break;
					}
					break;
				}
				break;
			}

			// Y軸回転
			D3DXVec3TransformCoord(&m_particle->move, &m_particle->move, matrixY);
		}
		m_particle = m_firstParticle;		// 先頭アドレスに

		// Y軸回転行列削除
		delete matrixY;
		matrixY = NULL;

		break;
	}
	case 1:			//パターン１は横に投げる
	{
		if (pow <= 0) return;

		// Y軸回転行列
		D3DXMATRIX *matrixY = new D3DXMATRIX;

		m_delay = 0;		// ずらす時間リセット
		m_numParticle = pow * MAX_WATER;		// パーティクル数

		// パーティクルの動的確保
		m_particle = new WATER_PARTICLE[m_numParticle];

		// 先頭アドレス保持
		m_firstParticle = m_particle;
		//配列の中を確認して未使用のパーティクルをセットする
		for (int i = 0; i < m_numParticle; i++, m_particle++)
		{
			//必要なものをセットしていく
			m_particle->bUse = true;
			m_particle->delay = rand() % 6;
			m_particle->pos = pos;
			m_particle->accel = D3DXVECTOR3(0.0f, -0.2f, 0.0f);

			D3DXMatrixRotationY(matrixY, (rot + (m_particle->delay - 3.0f) / -3.0f));			//時間差で回転を与える
			int angle = rand() % 360;
			float waterWidth = (rand() % 10) / 10.0f;
			float waterHeight = (rand() % 6) / 10.0f;
			m_particle->move = D3DXVECTOR3((cos(angle*D3DX_PI / 180)*waterWidth), 3.0f + (sin(angle*D3DX_PI / 180)*waterHeight), (rand() % 150) / 150.0f + 2.0f);

			// 当たり判定グループ
			for (int d = 0; d < 6; d++) {
				if (m_particle->delay != d) continue;
				for (int x = 0; x < 5; x++) {
					if (m_particle->move.x > colPartition1[0][x])	continue;
					for (int y = 0; y < 5; y++) {
						if (m_particle->move.y > colPartition1[1][y])	continue;
						for (int z = 0; z < 5; z++) {
							if (m_particle->move.z > colPartition1[2][z])	continue;
							// 登録
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

			// Y軸回転
			D3DXVec3TransformCoord(&m_particle->move, &m_particle->move, matrixY);
		}
		m_particle = m_firstParticle;		// 先頭アドレスに

		// Y軸回転行列削除
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
// 更新処理
// 戻り値：falseなら全パーティクルが消えている
//=============================================================================
bool CWaterAtk::Update(void)
{
	m_delay++;
	cnt = 0;
	if (m_delay > 1000) {
		return false;
	}
	bool deleteFlag = false;

	// 壁との衝突判定
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
		//未使用のパーティクルは使用しない
		if (!m_particle->bUse)
		{
			continue;
		}
		if (m_delay < m_particle->delay)	continue;

		//速度更新
		m_particle->move += m_particle->accel;

		//座標更新
		m_particle->pos += m_particle->move;

		//パーティクルの破棄
		if (m_particle->pos.y < 0)
		{
			m_particle->bUse = false;
			continue;
		}
		deleteFlag = true;
		particleNo[cnt] = i;
		cnt++;
	}
	m_particle = m_firstParticle;			// 先頭アドレスに
	return deleteFlag;
}
#include "debug.h"
//=============================================================================
// 描画処理
//=============================================================================
void CWaterAtk::Draw(int no)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	struct COLOR_VERTEX {
		D3DXVECTOR3 vecCoord;
		//	FLOAT w;
		FLOAT fScale;
		DWORD dwColor;
	};
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	// ポイントスプライトはディスプレイのPxで描画してしまう
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
	// 深度バッファ無効化
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// ライティング無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	pDevice->SetTexture(0, g_pD3DTextureWaterAtk);
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE);
	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_POINTLIST, cnt, pointSprite, sizeof(COLOR_VERTEX));
	SetRenderMode(BLEND_TRANSLUCENT);

	delete[] pointSprite;

	// ライティング有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
void CWaterAtk::CreateBillboard()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//// オブジェクトの頂点バッファを生成
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// 頂点データ用に確保するバッファサイズ(バイト単位)
	//	D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
	//	FVF_VERTEX_3D,				 // 使用する頂点フォーマット
	//	D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
	//	&g_pD3DVtxBuffBillboard,	// 頂点バッファインターフェースへのポインタ
	//	NULL);						// NULLに設定

	//{//頂点バッファの中身を埋める
	//	VERTEX_3D *pVtx;

	//	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	//	g_pD3DVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//	for (int nCntVtxY = 0; nCntVtxY < 1; nCntVtxY++)
	//	{
	//		for (int nCntVtxX = 0; nCntVtxX < 2; nCntVtxX++, pVtx += 2)
	//		{
	//			// 頂点座標の設定
	//			pVtx[0].vtx.x = -(0.5f) * WATER_SIZE + nCntVtxX * WATER_SIZE;
	//			pVtx[0].vtx.y = (0.5f) * WATER_SIZE - WATER_SIZE;
	//			pVtx[0].vtx.z = 0.0f;
	//			pVtx[1].vtx.x = -(0.5f) * WATER_SIZE + nCntVtxX * WATER_SIZE;
	//			pVtx[1].vtx.y = (0.5f) * WATER_SIZE;
	//			pVtx[1].vtx.z = 0.0f;

	//			// 法線の設定
	//			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//			// 反射光の設定
	//			pVtx[0].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	//			pVtx[1].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//			// テクスチャ座標の設定
	//			pVtx[0].tex.x = nCntVtxX;
	//			pVtx[0].tex.y = (nCntVtxY + 1);
	//			pVtx[1].tex.x = nCntVtxX;
	//			pVtx[1].tex.y = nCntVtxY;
	//		}
	//	}

	//	// 頂点データをアンロックする
	//	g_pD3DVtxBuffBillboard->Unlock();
	//}

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_WATER,			// ファイルの名前
		&g_pD3DTextureWaterAtk);	// 読み込むメモリー
}
void CWaterAtk::ReleaseBillboard() {
	//if (g_pD3DTextureWaterAtk != NULL)
	//{
	//	//頂点情報の解放
	//	g_pD3DTextureWaterAtk->Release();
	//	g_pD3DTextureWaterAtk = NULL;
	//}
	//SAFE_RELEASE(g_pD3DVtxBuffBillboard);
}

// パーティクルの位置を取得
D3DXVECTOR3 CWaterAtk::GetPos(int no, int *num) {
	if (m_colList[no].size() == 0) {
		*num = 0;
		return D3DXVECTOR3(9999, 9999, 9999);
	}
	*num = m_colList[no].size();
	return m_colList[no][0]->pos;
}


//パーティクルの数を取得
int CWaterAtk::GetNumParticle()
{
	return m_numParticle;
}

// パーティクルのグループ削除
void CWaterAtk::SetUseDown(int no) {
	for (int i = 0; i < m_colList[no].size(); i++) {
		m_colList[no][i]->bUse = false;
	}
	m_colList[no].clear();
}