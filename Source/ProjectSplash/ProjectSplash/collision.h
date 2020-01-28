// 衝突判定
#pragma once

#include "main.h"

//---------構造体定義------------


// 矩形同士の衝突判定
bool CollisionRect(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh);
// 円同士の衝突判定
bool CollisionCircle(float Ax, float Ay, float Ar, float Bx, float By, float Br);
// 線と線の衝突判定
bool CollisionLineLine(float A1x, float A1y, float A2x, float A2y, float B1x, float B1y, float B2x, float B2y);
// 矩形と線
bool CollisionRectLine(float left, float top, float right, float bottom, float x1, float y1, float x2, float y2);
// 円と線
bool CollisionCircleLine(float px, float py, float r, float ax, float ay, float bx, float by);
// 線と線のあたり判定、第五引数が交点
bool CrossLine(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, D3DXVECTOR2 pos3, D3DXVECTOR2 pos4, D3DXVECTOR2 *CrossP);
// AABB衝突判定
bool CollisionAABB(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2);
// レイと壁の当たり判定
//１：現在地のアドレス(In&Out)
//２：移動方向(In)
//３：地形モデルのメッシュ(In)
//４：壁との距離を格納するアドレス(Out)
bool CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 move, LPD3DXMESH mesh, float *dist);
// レイと地面の当たり判定
//１：現在地のアドレス(In&Out)
//２：地形モデルのメッシュ(In)
//３：壁との距離を格納するアドレス(Out)
bool CollisionStage(D3DXVECTOR3 *pos, LPD3DXMESH mesh, float *dist);

// レイと壁（水パーティクル用）
//１：現在地(In)
//２：移動方向
//３：地形モデルのメッシュ(In)
//４：当たった場所を格納するアドレス(Out)
//５：当たった場所が地面か(Out)
bool CollisionWaterStage(D3DXVECTOR3 pos, D3DXVECTOR3 move, LPD3DXMESH mesh, D3DXVECTOR3 *colPos, bool *ground);

// レイと壁（水パーティクル用）
//１：現在地(In)
//２：移動方向
//３：足元からレイを飛ばす位置までの移動量(In)
//４：地形モデルのメッシュ(In)
//５：距離(Out)
bool CollisionRayStage(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rayPos, LPD3DXMESH mesh, float *dist);

// 位置、向き、大きさ
bool CollisionOBB(D3DXVECTOR3 pos1, D3DXVECTOR3 direct1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 direct2, D3DXVECTOR3 size2);

class OBB
{
protected:

public:
	D3DXVECTOR3 m_Pos;              // 位置
	D3DXVECTOR3 m_NormaDirect[3];   // 方向ベクトル
	FLOAT m_fLength[3];             // 各軸方向の長さ
	D3DXVECTOR3 GetDirect(int elem) { return m_NormaDirect[elem]; }   // 指定軸番号の方向ベクトルを取得
	FLOAT GetLen_W(int elem) { return m_fLength[elem]; }          // 指定軸方向の長さを取得
	D3DXVECTOR3 GetPos_W() { return m_Pos; }             // 位置を取得
};