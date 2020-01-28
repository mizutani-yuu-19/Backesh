//=============================================================================
//
// カメラ処理 [camera.h]
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// 視野角
#define	VIEW_NEAR_Z			(10.0f)					// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(2000.0f)				// ビュー平面のFarZ値
//*****************************************************************************
// クラス
//*****************************************************************************
class CCamera
{
public:
	CCamera() {};
	//CCamera(int playerNum, int playerNo);
	~CCamera();
	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

	void SetCamera(void);
	static void GetViewMtx(D3DXMATRIX* pMtxView, int camNo);

	static float GetRotCamera(int no);
	static D3DXVECTOR3 GetPosCamera(int no) { return m_camera[no]->m_cameraPt; }	// カメラの視点
	static D3DXVECTOR3 GetLookatPosCamera(int no) { return m_camera[no]->m_lookatPt; }		// カメラの注視点
	static void CreatePlayerCamera(int playerNum);		// プレイヤーカメラ作成
	static void CreateStageCamera();		// ステージカメラ作成
	static void CreateResultCamera();		// リザルトカメラ作成
	static void SetCamera(int camNo);				// カメラセット
	static void UpdateCamera();
	static void DeleteCamera();
	static int	GetCameraNum() { return m_camera.size(); }						// カメラの数を返す
	static float GetAspect(int camNo) { return m_camera[camNo]->m_aspect; }						// アスペクト比
	static void ChangePlayer(int camNo,D3DXVECTOR3 fromPos);			// チェンジ
private:
protected:
	D3DXVECTOR3		m_cameraPt;					// カメラの視点
	D3DXVECTOR3		m_lookatPt;					// カメラの注視点
	D3DXVECTOR3		m_posCameraU;				// カメラの上方向
	float			m_dest;						// 視点と注視点の距離
	float			m_radX;						// カメラ左右角度
	float			m_radY;						// カメラ上下角度
	float			m_moveSpeed;
	D3DXMATRIX		m_mtxView;					// ビューマトリックス
	D3DXMATRIX		m_mtxProjection;			// プロジェクションマトリックス
	int				m_No;						// カメラナンバー

	D3DVIEWPORT9	m_port;						// 画面配置
	float			m_aspect;					// アスペクト比

	D3DXVECTOR3 m_changeFromPos;
	int			m_changeTime;

	static std::vector<CCamera*>	m_camera;
};

class CPlayerCamera :public CCamera
{
public:
	CPlayerCamera(int playerNum, int playerNo);
	~CPlayerCamera() {};

	void Init(void);
	void Uninit(void);
	void Update(void);


private:
};

class CStageCamera :public CCamera
{
public:
	CStageCamera();
	~CStageCamera() {};

	void Init(void);
	void Uninit(void);
	void Update(void);
private:
	float m_time;
};

class CResultCamera :public CCamera
{
public:
	CResultCamera();
	~CResultCamera() {};

	void Init(void);
	void Uninit(void);
	void Update(void);
private:
	float m_time;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
