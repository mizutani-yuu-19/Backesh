#include "MeshObj.h"

// �R���X�g���N�^
CMeshObj::CMeshObj()
{

}

// �f�X�g���N�^
CMeshObj::~CMeshObj()
{
}

// ������
void CMeshObj::Init()
{
}

// �X�V
void CMeshObj::Update()
{
}

// �`��
void CMeshObj::Draw(D3DXMATRIX world)
{
	m_mesh.Draw(world);
}

// �����������̂ݕ`��
void CMeshObj::DrawAlpha()
{
}

// �s���������̂ݕ`��
void CMeshObj::DrawNoAlpha(D3DXMATRIX world)
{
	m_mesh.Draw(world);
}

// ���
void CMeshObj::Fin()
{

}