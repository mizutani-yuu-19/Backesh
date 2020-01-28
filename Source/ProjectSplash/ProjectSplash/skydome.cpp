///////////////////////////////////////////////////
//
// スカイドーム
//
//////////////////////////////////////////////////
//----------------インクルード--------------
#include "skydome.h"
#include "camera.h"

CSkydome* CSkydome::m_skydome;				// 実体

// ステージ作成
// 引数：ステージ番号０又は１
void CSkydome::CreateSkydome(int type) {
	m_skydome = new CSkydome(type);
}

void CSkydome::DeleteSkydome() {
	SAFE_DELETE(m_skydome);
}

// コンストラクタ
CSkydome::CSkydome(int type) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	switch (type)
	{
	case 0:
		// ステージ1
		// 見た目モデル
		LoadModel(MODEL_SKYDOME1, &m_model);
		break;
	case 1:
		// ステージ1
		// 見た目モデル
		LoadModel(MODEL_SKYDOME2, &m_model);
		break;
	default:
		break;
	}
}

// デストラクタ
CSkydome::~CSkydome() {
	ReleaseModel(&m_model);
}

//-----------描画--------------
void CSkydome::Draw(int camNo) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl,mtxTrans;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXVECTOR3 camPos = CCamera::GetPosCamera(camNo);

	//スケールを反映
	D3DXMatrixScaling(&mtxScl, 20, 20, 20);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//移動を反映
	D3DXMatrixTranslation(&mtxTrans, camPos.x, camPos.y, camPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);  
	DrawModel(&m_model);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

