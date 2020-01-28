//=======================================================================================
//
//	  メッシュ クラス定義
//
//=======================================================================================
#pragma once
#include <windows.h>		// Windowsプログラムにはこれを付ける
#include <tchar.h>			// 汎用テキスト マッピング
#include <d3d9.h>			// DirectX Graphics 関連のヘッダ
#include <d3dx9.h>			// DirectX Graphics 関連のヘッダ

#include "MyHierarchy.h"

class CMesh
{
private:
	LPD3DXFRAME					m_pFrameRoot;	// ルート フレーム オブジェクト
	LPD3DXANIMATIONCONTROLLER	m_pAnimCtrl;	// アニメーション コントローラ オブジェクト
	UINT						m_uNumAnimSet;	// アニメーション セット数
	LPD3DXANIMATIONSET*			m_ppAnimSet;	// アニメーション セット
	CMyHierarchy				m_Hierarchy;	// 階層メモリ確保/解放クラス
	DWORD						m_dwPrev;		// 直前の時刻

	float						m_fRadius;		// 境界球 半径
	D3DXVECTOR3					m_vCenter;		// 境界球/境界ボックス 中心座標
	D3DXVECTOR3					m_vBBox;		// 境界ボックス 半分のサイズ

	float						m_fTotalTime;	// ループしないとき切るための時間
	float						m_fPlayTime;	// 再生する時間（－１でループ）

	float						m_speed;
public:
	D3DXMATRIX					m_handMatrix;	// 左手のマトリックス
	CMesh();									// コンストラクタ
	virtual ~CMesh();							// デストラクタ
	bool Initialize(LPCTSTR pszFName);			// メッシュ初期化
	void Finalize();							// メッシュ解放
	void Draw(D3DXMATRIX& world);				// メッシュ描画
	void SwitchAnimSet(UINT uAnimSet,float time);		// アニメーション切替
	void SetTime(double dTime);							// アニメーション開始時間設定
	void SetAnimSpeed(float speed);							// アニメーションスピード

	bool Intersect(D3DXVECTOR3& L0,
		D3DXVECTOR3& L1,
		bool bRay = false,
		D3DXVECTOR3* pCross = NULL,
		D3DXVECTOR3* pNormal = NULL);

	float GetRadius() { return m_fRadius; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	D3DXVECTOR3& GetCenter() { return m_vCenter; }
	void SetCenter(LPD3DXVECTOR3 pvCenter);
	D3DXVECTOR3& GetBBox() { return m_vBBox; }
	void SetBBox(LPD3DXVECTOR3 pvBBox);

private:
	void RenderMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
	void DrawFrame(LPD3DXFRAME pFrame);
	HRESULT AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase);
	HRESULT AllocAllBoneMatrix(LPD3DXFRAME pFrameBase);

	bool IntersectFrame(LPD3DXFRAME pFrameBase,
		D3DXVECTOR3& L0, D3DXVECTOR3& W, bool bRay,
		D3DXVECTOR3* pCross, D3DXVECTOR3* pNormal);
	bool IntersectMeshContainer(
		LPD3DXMESHCONTAINER pMeshContainer,
		D3DXVECTOR3& L0, D3DXVECTOR3& W, bool bRay,
		D3DXVECTOR3* pCross, D3DXVECTOR3* pNormal);

	void CalcCollision(LPD3DXFRAME pFrame);
	void CalcCollisionFrame(LPD3DXFRAME pFrame);
	void CalcCollisionMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, LPD3DXFRAME pFrame);
};

//=======================================================================================
//	End of File
//=======================================================================================