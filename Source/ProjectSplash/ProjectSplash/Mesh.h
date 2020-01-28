//=======================================================================================
//
//	  ���b�V�� �N���X��`
//
//=======================================================================================
#pragma once
#include <windows.h>		// Windows�v���O�����ɂ͂����t����
#include <tchar.h>			// �ėp�e�L�X�g �}�b�s���O
#include <d3d9.h>			// DirectX Graphics �֘A�̃w�b�_
#include <d3dx9.h>			// DirectX Graphics �֘A�̃w�b�_

#include "MyHierarchy.h"

class CMesh
{
private:
	LPD3DXFRAME					m_pFrameRoot;	// ���[�g �t���[�� �I�u�W�F�N�g
	LPD3DXANIMATIONCONTROLLER	m_pAnimCtrl;	// �A�j���[�V���� �R���g���[�� �I�u�W�F�N�g
	UINT						m_uNumAnimSet;	// �A�j���[�V���� �Z�b�g��
	LPD3DXANIMATIONSET*			m_ppAnimSet;	// �A�j���[�V���� �Z�b�g
	CMyHierarchy				m_Hierarchy;	// �K�w�������m��/����N���X
	DWORD						m_dwPrev;		// ���O�̎���

	float						m_fRadius;		// ���E�� ���a
	D3DXVECTOR3					m_vCenter;		// ���E��/���E�{�b�N�X ���S���W
	D3DXVECTOR3					m_vBBox;		// ���E�{�b�N�X �����̃T�C�Y

	float						m_fTotalTime;	// ���[�v���Ȃ��Ƃ��؂邽�߂̎���
	float						m_fPlayTime;	// �Đ����鎞�ԁi�|�P�Ń��[�v�j

	float						m_speed;
public:
	D3DXMATRIX					m_handMatrix;	// ����̃}�g���b�N�X
	CMesh();									// �R���X�g���N�^
	virtual ~CMesh();							// �f�X�g���N�^
	bool Initialize(LPCTSTR pszFName);			// ���b�V��������
	void Finalize();							// ���b�V�����
	void Draw(D3DXMATRIX& world);				// ���b�V���`��
	void SwitchAnimSet(UINT uAnimSet,float time);		// �A�j���[�V�����ؑ�
	void SetTime(double dTime);							// �A�j���[�V�����J�n���Ԑݒ�
	void SetAnimSpeed(float speed);							// �A�j���[�V�����X�s�[�h

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