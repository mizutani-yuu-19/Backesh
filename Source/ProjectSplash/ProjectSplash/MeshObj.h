#pragma once
#include "Mesh.h"

class CMeshObj
{
protected:
	CMesh	m_mesh;
	D3DXMATRIX* GetHandMatrix() { return &m_mesh.m_handMatrix; }

public:
	CMeshObj();
	virtual ~CMeshObj();

	//void SetMesh(CMesh* pMesh = NULL);
	virtual void Init();	// 初期化
	virtual void Update();	// 更新
	void Draw(D3DXMATRIX world);	// 描画
	virtual void DrawAlpha();
	virtual void DrawNoAlpha(D3DXMATRIX world);
	virtual void Fin();		// 解放
};

