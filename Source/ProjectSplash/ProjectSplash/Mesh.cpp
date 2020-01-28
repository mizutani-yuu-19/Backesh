//=======================================================================================
//
//	  ���b�V�� �N���X
//
//=======================================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"
#include "Mesh.h"

//---------------------------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------------------------
CMesh::CMesh()
{
	m_pFrameRoot = NULL;
	m_pAnimCtrl = NULL;
	m_uNumAnimSet = 0;
	m_ppAnimSet = NULL;
	m_fTotalTime = 0.0f;
	m_fPlayTime = 0.0f;
	m_speed = 1000.0f;
}

//---------------------------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------------------------
CMesh::~CMesh()
{
}

//---------------------------------------------------------------------------------------
// ���b�V��������
//---------------------------------------------------------------------------------------
bool CMesh::Initialize(LPCTSTR pszFName)
{
	// �f�B���N�g�����o
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(pszFName, szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	m_Hierarchy.SetDirectory(szDir);

	// �K�w�\�����b�V���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshHierarchyFromX(pszFName,
		D3DXMESH_MANAGED, GetDevice(),
		&m_Hierarchy, NULL,
		&m_pFrameRoot, &m_pAnimCtrl);
	if (FAILED(hr)) return false;

	// �{�[���ƃt���[���̊֘A�t��
	hr = AllocAllBoneMatrix(m_pFrameRoot);
	if (FAILED(hr)) return false;

	// �A�j���[�V�����Z�b�g�擾
	m_uNumAnimSet = 0;
	if (m_pAnimCtrl) {
		m_uNumAnimSet = m_pAnimCtrl->GetNumAnimationSets();
		if (m_uNumAnimSet > 0) {
			m_ppAnimSet = new LPD3DXANIMATIONSET[m_uNumAnimSet];
			for (DWORD u = 0; u < m_uNumAnimSet; ++u) {
				m_pAnimCtrl->GetAnimationSet(u, &m_ppAnimSet[u]);
			}
		}
	}

	if (m_pFrameRoot) {
		// �}�g���b�N�X�X�V
		SetTime(0.0);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		UpdateFrameMatrices(m_pFrameRoot, &world);

		// ���E��/���E�{�b�N�X�擾
		CalcCollision(m_pFrameRoot);
	}

	// �o�ߎ��Ԍv���p�����ݒ�
	m_dwPrev = ::timeGetTime();

	return SUCCEEDED(hr);
}

//---------------------------------------------------------------------------------------
// ���b�V�����
//---------------------------------------------------------------------------------------
void CMesh::Finalize()
{
	// �A�j���[�V�����j��
	if (m_ppAnimSet) {
		for (DWORD u = 0; u < m_uNumAnimSet; ++u) {
			SAFE_RELEASE(m_ppAnimSet[u]);
		}
		SAFE_DELETE_ARRAY(m_ppAnimSet);
	}
	SAFE_RELEASE(m_pAnimCtrl);

	// ���b�V���j��
	if (m_pFrameRoot) {
		D3DXFrameDestroy(m_pFrameRoot, &m_Hierarchy);
		m_pFrameRoot = NULL;
	}
}

//---------------------------------------------------------------------------------------
// ���b�V���`��
//---------------------------------------------------------------------------------------
void CMesh::Draw(D3DXMATRIX& world)
{
	// �A�j���[�V�����X�V
	if (m_pAnimCtrl) {
		DWORD dwNow = ::timeGetTime();
		double d = (dwNow - m_dwPrev) / m_speed;
		m_dwPrev = dwNow;
		m_fTotalTime += d;
		if (m_fPlayTime>m_fTotalTime||m_fPlayTime<0) {
			m_pAnimCtrl->AdvanceTime(d, NULL);
		}
	}

	// �}�g���b�N�X�X�V
	GetDevice()->SetTransform(D3DTS_WORLD, &world);
	if (m_pFrameRoot) {
		UpdateFrameMatrices(m_pFrameRoot, &world);
		// �t���[���`��
		DrawFrame(m_pFrameRoot);
	}
}
//---------------------------------------------------------------------------------------
// �A�j���[�V�����X�s�[�h�̕ύX
//---------------------------------------------------------------------------------------
void CMesh::SetAnimSpeed(float speed) {
	if (speed < 0)	m_speed = 0.0f;
	else	m_speed = 1000.0f / speed;
}

//---------------------------------------------------------------------------------------
// �t���[���̃}�g���b�N�X���X�V
//---------------------------------------------------------------------------------------
void CMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	// �e�̃}�g���b�N�X���|�����킹��
	if (pParentMatrix) {
		pFrame->CombinedTransformationMatrix =
			pFrame->TransformationMatrix * *pParentMatrix;
	}
	else {
		pFrame->CombinedTransformationMatrix =
			pFrame->TransformationMatrix;
	}
	// �Z��t���[��������΃}�g���b�N�X���X�V
	if (pFrame->pFrameSibling) {
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	// �q�t���[��������΃}�g���b�N�X���X�V
	if (pFrame->pFrameFirstChild) {
		// �������e�ƂȂ�̂ŁA�����̃}�g���b�N�X��n��
		UpdateFrameMatrices(pFrame->pFrameFirstChild,
			&pFrame->CombinedTransformationMatrix);
	}
	if (pFrame->Name[0] == 'L') {
		m_handMatrix = pFrame->CombinedTransformationMatrix;
	}
	
}

//---------------------------------------------------------------------------------------
// �t���[���̕`��
//---------------------------------------------------------------------------------------
void CMesh::DrawFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer) {
		// ���b�V���R���e�i�̕`��
		RenderMeshContainer(pMeshContainer, pFrame);
		// ���̃��b�V���R���e�i
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	// �Z��t���[��������ΌZ��t���[����`��
	if (pFrame->pFrameSibling)
		DrawFrame(pFrame->pFrameSibling);
	// �q�t���[��������Ύq�t���[����`��
	if (pFrame->pFrameFirstChild)
		DrawFrame(pFrame->pFrameFirstChild);
}

//---------------------------------------------------------------------------------------
// ���b�V���R���e�i�̕`��
//---------------------------------------------------------------------------------------
void CMesh::RenderMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	if (pMeshContainer->pSkinInfo) {
		// �X�L�����b�V�����f��
		LPD3DXBONECOMBINATION pBoneCombi =
			(LPD3DXBONECOMBINATION)
			pMeshContainer->pBoneBuffer->GetBufferPointer();

		DWORD dwPrevBoneID = UINT_MAX;
		DWORD dwBlendMatrix;
		for (DWORD i = 0; i < pMeshContainer->dwBone; ++i) {
			dwBlendMatrix = 0;
			for (DWORD k = 0; k < pMeshContainer->dwWeight; ++k) {
				if (pBoneCombi[i].BoneId[k] != UINT_MAX)
					dwBlendMatrix = k;
			}
			GetDevice()->SetRenderState(
				D3DRS_VERTEXBLEND, dwBlendMatrix);
			for (DWORD k = 0; k < pMeshContainer->dwWeight; ++k) {
				DWORD id = pBoneCombi[i].BoneId[k];
				if (id != UINT_MAX) {
					GetDevice()->SetTransform(
						D3DTS_WORLDMATRIX(k),
						&(pMeshContainer->pBoneOffsetMatrix[id] *
							*pMeshContainer->ppBoneMatrix[id]));
				}
			}
			GetDevice()->SetMaterial(&pMeshContainer->pMaterials[pBoneCombi[i].AttribId].MatD3D);
			GetDevice()->SetTexture(0, pMeshContainer->ppTextures[pBoneCombi[i].AttribId]);
			dwPrevBoneID = pBoneCombi[i].AttribId;
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		return;
	}
	// �X�L���������f��
	GetDevice()->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
	for (DWORD iAttrib = 0; iAttrib < pMeshContainer->NumMaterials; ++iAttrib) {
		DWORD dwAttrib = pMeshContainer->pAttributeTable[iAttrib].AttribId;
		GetDevice()->SetMaterial(&pMeshContainer->pMaterials[dwAttrib].MatD3D);
		GetDevice()->SetTexture(0, pMeshContainer->ppTextures[dwAttrib]);
		pMeshContainer->MeshData.pMesh->DrawSubset(dwAttrib);
	}
}

//---------------------------------------------------------------------------------------
// �{�[���p���[���h�E�}�g���b�N�X�̈�m��
//---------------------------------------------------------------------------------------
HRESULT CMesh::AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	if (pMeshContainer->pSkinInfo == NULL)
		return S_OK;	// �X�L����񂪖�����Ή������Ȃ�
	DWORD dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->ppBoneMatrix = new LPD3DXMATRIX[dwBoneNum];
	for (DWORD i = 0; i < dwBoneNum; ++i) {
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(m_pFrameRoot,
			pMeshContainer->pSkinInfo->GetBoneName(i));
		if (pFrame == NULL)
			return E_FAIL;
		pMeshContainer->ppBoneMatrix[i] =
			&pFrame->CombinedTransformationMatrix;
	}
	return S_OK;
}

HRESULT CMesh::AllocAllBoneMatrix(LPD3DXFRAME pFrameBase)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->pMeshContainer) {
		hr = AllocBoneMatrix(pFrame->pMeshContainer);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameSibling) {
		hr = AllocAllBoneMatrix(pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameFirstChild) {
		hr = AllocAllBoneMatrix(pFrame->pFrameFirstChild);
	}
	return hr;
}

//---------------------------------------------------------------------------------------
// �A�j���[�V�����ؑ�
//---------------------------------------------------------------------------------------
void CMesh::SwitchAnimSet(UINT uAnimSet, float time)
{
	if (uAnimSet >= m_uNumAnimSet || m_pAnimCtrl == NULL) return;
	m_pAnimCtrl->SetTrackAnimationSet(0, m_ppAnimSet[uAnimSet]);
	m_fPlayTime = time;
	m_fTotalTime = 0.0f;
}

//---------------------------------------------------------------------------------------
// �A�j���[�V�����J�n���Ԑݒ�
//---------------------------------------------------------------------------------------
void CMesh::SetTime(double dTime)
{
	if (m_pAnimCtrl == NULL) return;
	for (DWORD i = 0; i < m_pAnimCtrl->GetMaxNumTracks(); ++i) {
		m_pAnimCtrl->SetTrackPosition(i, 0);
	}
	m_pAnimCtrl->ResetTime();
	m_pAnimCtrl->AdvanceTime(dTime, NULL);
}

void CMesh::SetCenter(LPD3DXVECTOR3 pvCenter) {
	if (pvCenter) {
		m_vCenter = *pvCenter;
	}
	else {
		m_vCenter = D3DXVECTOR3(0, 0, 0);
	}
}

void CMesh::SetBBox(LPD3DXVECTOR3 pvBBox)
{
	if (pvBBox) {
		m_vBBox = *pvBBox;
	}
	else {
		m_vBBox = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	}
}

// ���E��/���E�{�b�N�X�擾
void CMesh::CalcCollision(LPD3DXFRAME pFrame)
{
	D3DXVECTOR3& vMax = m_vBBox;
	D3DXVECTOR3& vMin = m_vCenter;
	vMax.x = vMax.y = vMax.z = -FLT_MAX;
	vMin.x = vMin.y = vMin.z = FLT_MAX;
	m_fRadius = -1.0f;
	CalcCollisionFrame(pFrame);
	D3DXVECTOR3 vBBox, vCenter;
	vBBox = (vMax - vMin) / 2.0f;
	vCenter = (vMax + vMin) / 2.0f;
	m_vBBox = vBBox;
	m_vCenter = vCenter;
	m_fRadius = 0.0f;
	CalcCollisionFrame(pFrame);
}

// �t���[�����̒��_���W�̒��o
void CMesh::CalcCollisionFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer) {
		// ���b�V���R���e�i���̒��_���W�̒��o
		CalcCollisionMeshContainer(pMeshContainer, pFrame);
		// ���̃��b�V���R���e�i
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	// �Z��t���[��������ΌZ��t���[����`��
	if (pFrame->pFrameSibling)
		CalcCollisionFrame(pFrame->pFrameSibling);
	// �q�t���[��������Ύq�t���[����`��
	if (pFrame->pFrameFirstChild)
		CalcCollisionFrame(pFrame->pFrameFirstChild);
}

// ���b�V���R���e�i���̒��_���W�̒��o
void CMesh::CalcCollisionMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, LPD3DXFRAME pFrame)
{
	D3DXMATRIX& matrix = ((MYFRAME*)pFrame)->CombinedTransformationMatrix;
	// ���_���W�̒��o
	LPD3DXMESH pMesh = pMeshContainer->MeshData.pMesh;
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVtx = pMesh->GetNumVertices();
	LPBYTE pVtx;
	D3DXVECTOR3 vtx;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	for (DWORD i = 0; i < dwVtx; ++i, pVtx += dwStride) {
		D3DXVec3TransformCoord(&vtx, (LPD3DXVECTOR3)pVtx, &matrix);
		if (m_fRadius < 0.0f) {
			D3DXVECTOR3& vMax = m_vBBox;
			D3DXVECTOR3& vMin = m_vCenter;
			if (vMax.x < vtx.x) vMax.x = vtx.x;
			if (vMax.y < vtx.y) vMax.y = vtx.y;
			if (vMax.z < vtx.z) vMax.z = vtx.z;
			if (vMin.x > vtx.x) vMin.x = vtx.x;
			if (vMin.y > vtx.y) vMin.y = vtx.y;
			if (vMin.z > vtx.z) vMin.z = vtx.z;
		}
		else {
			float fRadius = D3DXVec3Length(&(vtx - m_vCenter));
			if (m_fRadius < fRadius)
				m_fRadius = fRadius;
		}
	}
	pMesh->UnlockVertexBuffer();
}

//=======================================================================================
//	End of File
//=======================================================================================