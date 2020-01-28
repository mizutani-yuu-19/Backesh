//=============================================================================
//
// �J�������� [camera.h]
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// ����p
#define	VIEW_NEAR_Z			(10.0f)					// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(2000.0f)				// �r���[���ʂ�FarZ�l
//*****************************************************************************
// �N���X
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
	static D3DXVECTOR3 GetPosCamera(int no) { return m_camera[no]->m_cameraPt; }	// �J�����̎��_
	static D3DXVECTOR3 GetLookatPosCamera(int no) { return m_camera[no]->m_lookatPt; }		// �J�����̒����_
	static void CreatePlayerCamera(int playerNum);		// �v���C���[�J�����쐬
	static void CreateStageCamera();		// �X�e�[�W�J�����쐬
	static void CreateResultCamera();		// ���U���g�J�����쐬
	static void SetCamera(int camNo);				// �J�����Z�b�g
	static void UpdateCamera();
	static void DeleteCamera();
	static int	GetCameraNum() { return m_camera.size(); }						// �J�����̐���Ԃ�
	static float GetAspect(int camNo) { return m_camera[camNo]->m_aspect; }						// �A�X�y�N�g��
	static void ChangePlayer(int camNo,D3DXVECTOR3 fromPos);			// �`�F���W
private:
protected:
	D3DXVECTOR3		m_cameraPt;					// �J�����̎��_
	D3DXVECTOR3		m_lookatPt;					// �J�����̒����_
	D3DXVECTOR3		m_posCameraU;				// �J�����̏����
	float			m_dest;						// ���_�ƒ����_�̋���
	float			m_radX;						// �J�������E�p�x
	float			m_radY;						// �J�����㉺�p�x
	float			m_moveSpeed;
	D3DXMATRIX		m_mtxView;					// �r���[�}�g���b�N�X
	D3DXMATRIX		m_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	int				m_No;						// �J�����i���o�[

	D3DVIEWPORT9	m_port;						// ��ʔz�u
	float			m_aspect;					// �A�X�y�N�g��

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
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
