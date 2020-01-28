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
	virtual void Init();	// ‰Šú‰»
	virtual void Update();	// XV
	void Draw(D3DXMATRIX world);	// •`‰æ
	virtual void DrawAlpha();
	virtual void DrawNoAlpha(D3DXMATRIX world);
	virtual void Fin();		// ‰ğ•ú
};

