#include "MeshObj.h"

// コンストラクタ
CMeshObj::CMeshObj()
{

}

// デストラクタ
CMeshObj::~CMeshObj()
{
}

// 初期化
void CMeshObj::Init()
{
}

// 更新
void CMeshObj::Update()
{
}

// 描画
void CMeshObj::Draw(D3DXMATRIX world)
{
	m_mesh.Draw(world);
}

// 半透明部分のみ描画
void CMeshObj::DrawAlpha()
{
}

// 不透明部分のみ描画
void CMeshObj::DrawNoAlpha(D3DXMATRIX world)
{
	m_mesh.Draw(world);
}

// 解放
void CMeshObj::Fin()
{

}