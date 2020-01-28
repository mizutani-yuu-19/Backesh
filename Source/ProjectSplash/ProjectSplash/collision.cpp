// 衝突判定
#include "collision.h"
#include <stdlib.h>
#include <math.h>

// 矩形同士の衝突判定
bool CollisionRect(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh) {
	return (Ax <= Bx + Bw && Bx <= Ax + Aw && Ay <= By + Bh && By <= Ay + Ah);
}
// 円同士の衝突判定
bool CollisionCircle(float Ax, float Ay, float Ar, float Bx, float By, float Br) {
	float dx = Ax - Bx;
	float dy = Ay - By;
	float r = Ar + Br;
	return (dx*dx + dy*dy <= r*r);
}
// 線と線
bool CollisionLineLine(float A1x, float A1y, float A2x, float A2y, float B1x, float B1y, float B2x, float B2y) {
	{
		float baseX = B2x - B1x;
		float baseY = B2y - B1y;
		float sub1X = A1x - B1x;
		float sub1Y = A1y - B1y;
		float sub2X = A2x - B1x;
		float sub2Y = A2y - B1y;

		float bs1 = baseX * sub1Y - baseY * sub1X;
		float bs2 = baseX * sub2Y - baseY * sub2X;
		float re = bs1 * bs2;
		if (re > 0) {
			return false;
		}
	}
	{
		float baseX = A2x - A1x;
		float baseY = A2y - A1y;
		float sub1X = B1x - A1x;
		float sub1Y = B1y - A1y;
		float sub2X = B2x - A1x;
		float sub2Y = B2y - A1y;

		float bs1 = baseX * sub1Y - baseY * sub1X;
		float bs2 = baseX * sub2Y - baseY * sub2X;
		float re = bs1 * bs2;
		if (re > 0) {
			return false;
		}
	}
	return true;
}
// 矩形と線
bool CollisionRectLine(float left, float top, float right, float bottom, float x1, float y1, float x2, float y2) {
	if (CollisionLineLine(left, top, right, top, x1, y1, x2, y2))
		return true;
	if (CollisionLineLine(right, top, right, bottom, x1, y1, x2, y2))
		return true;
	if (CollisionLineLine(right, bottom, left, bottom, x1, y1, x2, y2))
		return true;
	if (CollisionLineLine(left, bottom, left, top, x1, y1, x2, y2))
		return true;

	return false;
}
// 円と線
bool CollisionCircleLine(float px, float py, float r, float ax, float ay, float bx, float by) {
	if (r < 0)	return false;
	// ABの角度-APの角度=θ
	float sita = atan2(ay - by, ax - bx) - atan2(ay - py, ax - px);
	if (sita < 0)	sita*-1.0f;
	// ABの長さ
	float lenAB = sqrt((ax - bx)*(ax - bx) + (ay - by)*(ay - by));
	if (lenAB < 0)	lenAB *= -1.0f;	// 絶対値変換
	// APの長さ
	float lenAP = sqrt((ax - px)*(ax - px) + (ay - py)*(ay - py));
	if (lenAP < 0)	lenAP *= -1.0f;	// 絶対値変換
	// 内積AX
	float lenAX = lenAP*cos(sita);
	//線分APとPの最短距離
	float shortDist = 0;
	if (lenAX < 0) {
		//AXが負なら APが円の中心までの最短距離
		//shortDist = lenAP;
		shortDist = 1000;
	}
	else if (lenAX > lenAB) {
		//AXがAPよりも長い場合は、BPが円の中心までの最短距離
		//shortDist = lenAB;
		shortDist = 1000;
	}
	else {
		//PがAB上にあるので、XPが最短距離
		float lenXP = lenAP*sin(sita);
		if (lenXP < 0)	lenXP *= -1.0f;
		shortDist = lenXP;
	}
	// 最短距離が半径より短ければ
	if (shortDist < r) {
		return true;
	}
	return false;
}

// 線と線のあたり判定、第五引数は交点
//http://mf-atelier.sakura.ne.jp/mf-atelier/modules/tips/program/algorithm/a1.html
bool CrossLine(
	D3DXVECTOR2 pos1,
	D3DXVECTOR2 pos2,
	D3DXVECTOR2 pos3,
	D3DXVECTOR2 pos4,
	D3DXVECTOR2 *CrossP
)
{
	double ksi, eta, delta;
	double ramda, mu;

	ksi = (pos4.y - pos3.y)*(pos4.x - pos1.x) - (pos4.x - pos3.x)*(pos4.y - pos1.y);

	eta = (pos2.x - pos1.x)*(pos4.y - pos1.y) - (pos2.y - pos1.y)*(pos4.x - pos1.x);

	delta = (pos2.x - pos1.x)*(pos4.y - pos3.y) - (pos2.y - pos1.y)*(pos4.x - pos3.x);

	ramda = ksi / delta;
	mu = eta / delta;

	if ((ramda >= 0 && ramda <= 1) && (mu >= 0 && mu <= 1))
	{
		CrossP->x = pos1.x + ramda*(pos2.x - pos1.x);
		CrossP->y = pos1.y + ramda*(pos2.y - pos1.y);

		return TRUE;
	}

	return FALSE;
}

//AABB衝突判定
bool CollisionAABB(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
{
	D3DXVECTOR3 min1;		//比較対象１の最小座標
	D3DXVECTOR3 max2;		//比較対象２の最大座標
	min1 = pos1 - size1 * 0.5f;
	max2 = pos2 + size2 * 0.5f;

	//１のX最小座標　＞　２のZ最大座標の場合衝突していない
	if (min1.x > max2.x) return false;

	D3DXVECTOR3 max1;		//比較対象１の最大座標
	D3DXVECTOR3 min2;		//比較対象２の最小座標
	max1 = pos1 + size1 * 0.5f;
	min2 = pos2 - size2 * 0.5f;

	//１のX最大座標　＞　２のX最小座標の場合衝突していない
	if (max1.x < min2.x) return false;
	//１のY最小座標　＞　２のY最大座標の場合衝突していない
	if (min1.y > max2.y) return false;
	//１のY最大座標　＞　２のY最小座標の場合衝突していない
	if (max1.y < min2.y) return false;
	//１のZ最小座標　＞　２のZ最大座標の場合衝突していない
	if (min1.z > max2.z) return false;
	//１のZ最大座標　＞　２のZ最小座標の場合衝突していない
	if (max1.z < min2.z) return false;

	//上記のどのパターンにも入らなかったら衝突
	return true;
}


// レイと壁の当たり判定
bool CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 move, LPD3DXMESH mesh, float *dist) {
	if (move == D3DXVECTOR3(0.0f, 0.0f, 0.0f))	return false;
	BOOL hit = false;		// 当たったかフラグ
	DWORD *index = new DWORD;			// 最短へのインデックス
	FLOAT u, v;			// 面のどの位置に当たったか
	//D3DXVECTOR3 colP;	// 衝突ポイント
	float _dist;
	D3DXVECTOR3 _move;
	D3DXVec3Normalize(&_move, &move);
	D3DXVECTOR3 beforePos = *pos;// +_move * 10.0f;
	D3DXVECTOR3 afterPos = *pos + move;
	beforePos.y += 10.0f;
	D3DXVECTOR3 tempMove = move;
	tempMove.y = 0.0f;
	afterPos.y += 10.0f;
	// 当たり判定
	D3DXIntersect(mesh, &beforePos, &tempMove, &hit, index, &u, &v, dist, NULL, NULL);
	if (!hit) {
		*dist = 9999;
		SAFE_DELETE(index);
		return false;
	}
	if (hit&&*dist < 10.0f) {
		struct FVF {
			D3DXVECTOR3 pos;
			D3DXVECTOR3 nor;
		}*vertex;
		WORD *indexInf;
		LPDIRECT3DVERTEXBUFFER9  vertexBuffer;
		mesh->GetVertexBuffer(&vertexBuffer);
		LPDIRECT3DINDEXBUFFER9  indexBuffer;
		mesh->GetIndexBuffer(&indexBuffer);
		indexBuffer->Lock(0, 0, (void**)&indexInf, 0);
		vertexBuffer->Lock(0, 0, (void**)&vertex, 0);

		// 当たった三角形
		FVF	polyVertex[3];
		for (int i = 0; i < 3; i++) {
			polyVertex[i] = vertex[indexInf[*index * 3 + i]];
		}

		vertexBuffer->Unlock();
		indexBuffer->Unlock();

		D3DXVECTOR3 planeNor;
		D3DXVECTOR3 vecA = polyVertex[1].pos - polyVertex[0].pos;
		D3DXVECTOR3 vecB = polyVertex[2].pos - polyVertex[0].pos;

		D3DXVec3Cross(&planeNor, &vecA, &vecB);
		D3DXVec3Normalize(&planeNor, &planeNor);

		// 平面作成
		D3DXPLANE	plane;
		D3DXPlaneFromPoints(&plane, &polyVertex[0].pos, &polyVertex[1].pos, &polyVertex[2].pos);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	移動前後の座標がポリゴンを挟んでいるか判定し、はさんでいれば当たったとみなし、すべり処理を行う


		//	移動後、前の位置が平面をはさむかチェック
		float ans[2];	//	数学の平面と頂点に関する公式を利用する：平面の表面にあるなら正：裏面なら負になる
		ans[0] = (plane.a * afterPos.x) + (plane.b * afterPos.y) + (plane.c * afterPos.z) + plane.d;
		ans[1] = (plane.a * beforePos.x) + (plane.b * beforePos.y) + (plane.c * beforePos.z) + plane.d;

		//	二つの値の正負が反対ならば、両面をはさむ->当たっているのですべり処理を行う
		if (ans[0] * ans[1] < 0.0f) {
			//	移動後の座標からポリゴン法線方向にレイを飛ばし、ポリゴンとの交点を求める
			D3DXIntersect(mesh, &afterPos, &planeNor, &hit, index, NULL, NULL, &_dist, NULL, NULL);
			// Y軸回転行列
			D3DXMATRIX *matrixY = new D3DXMATRIX;
			D3DXVECTOR3 tempNor;
			float tempdist;
			BOOL _hit;
			D3DXMatrixRotationY(matrixY, D3DX_PI*0.5f);
			D3DXVec3TransformCoord(&tempNor, &planeNor, matrixY);
			SAFE_DELETE(matrixY);
			D3DXIntersect(mesh, &beforePos, &tempNor, &_hit, index, NULL, NULL, &tempdist, NULL, NULL);
			if (_hit&&tempdist < 5.0f) {
				pos->y += move.y; 
				SAFE_DELETE(index);
				return true;
			}
			else {
				tempNor.x *= -1.0f;
				tempNor.z *= -1.0f;
				D3DXIntersect(mesh, &beforePos, &tempNor, &_hit, index, NULL, NULL, &tempdist, NULL, NULL);
				SAFE_DELETE(index);
				SAFE_DELETE(matrixY);
				if (_hit&&tempdist < 5.0f) {
					pos->y += move.y;
					return true;
				}
			}
			//	交差していた場合は、レイ表示情報の終点位置を更新する
			if (hit&&_dist < 10.0f)
			{
				//	すべり後の座標ポリゴンの表面より少し多めに戻す
				afterPos = afterPos + planeNor*(_dist + 1.0f);

				//	すべての処理がすんだ座標をセット
				*pos = afterPos;
				pos->y -= 10.0f;
				SAFE_DELETE(index);
				return true;
			}
			else {

			}
		}
	}
	SAFE_DELETE(index);
	return false;
}

// レイと地面の当たり判定
bool CollisionStage(D3DXVECTOR3 *pos, LPD3DXMESH mesh, float *dist) {
	BOOL hit = false;		// 当たったかフラグ
	DWORD *index = new DWORD;			// 最短へのインデックス
	FLOAT u, v;			// 面のどの位置に当たったか
	D3DXVECTOR3 colP;	// 衝突ポイント
	// 正規化
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	D3DXVECTOR3 beforePos = *pos;
	beforePos.y += 10.0f;
	// 当たり判定
	D3DXIntersect(mesh, &beforePos, &move, &hit, index, &u, &v, dist, NULL, NULL);
	if (hit) {
		struct FVF {
			D3DXVECTOR3 pos;
			D3DXVECTOR3 nor;
		}*vertex;
		WORD *indexInf;
		LPDIRECT3DVERTEXBUFFER9  vertexBuffer;
		mesh->GetVertexBuffer(&vertexBuffer);
		LPDIRECT3DINDEXBUFFER9  indexBuffer;
		mesh->GetIndexBuffer(&indexBuffer);
		indexBuffer->Lock(0, 0, (void**)&indexInf, 0);
		vertexBuffer->Lock(0, 0, (void**)&vertex, 0);

		// 当たった三角形
		FVF	polyVertex[3];
		for (int i = 0; i < 3; i++) {
			polyVertex[i] = vertex[indexInf[*index * 3 + i]];
		}

		vertexBuffer->Unlock();
		indexBuffer->Unlock();

		// ぶつかったらめり込み量分戻す

		D3DXVECTOR3 planeNor;
		D3DXVECTOR3 vecA = polyVertex[1].pos - polyVertex[0].pos;
		D3DXVECTOR3 vecB = polyVertex[2].pos - polyVertex[0].pos;

		D3DXVec3Cross(&planeNor, &vecA, &vecB);
		D3DXVec3Normalize(&planeNor, &planeNor);

		colP = polyVertex[0].pos + u*vecA + v*vecB;
		colP.y -= 10.0f;
		//	D3DXVECTOR3	len = *pos - colP;
		if (*dist <= 10.0f) {
			pos->y = colP.y + planeNor.y*10.0f;
		}
	}
	SAFE_DELETE(index);
	return hit;
}

// レイと壁（水パーティクル用）
bool CollisionWaterStage(D3DXVECTOR3 pos, D3DXVECTOR3 move, LPD3DXMESH mesh, D3DXVECTOR3 *colPos, bool *ground) {
	BOOL hit = false;		// 当たったかフラグ
	DWORD *index = new DWORD;			// 最短へのインデックス
	FLOAT u, v;			// 面のどの位置に当たったか
	FLOAT dist;
	D3DXIntersect(mesh, &pos, &move, &hit, index, &u, &v, &dist, NULL, NULL);
	if (hit&&dist < 10.0f) {
		struct FVF {
			D3DXVECTOR3 pos;
			D3DXVECTOR3 nor;
		}*vertex;
		WORD *indexInf;
		LPDIRECT3DVERTEXBUFFER9  vertexBuffer;
		mesh->GetVertexBuffer(&vertexBuffer);
		LPDIRECT3DINDEXBUFFER9  indexBuffer;
		mesh->GetIndexBuffer(&indexBuffer);
		indexBuffer->Lock(0, 0, (void**)&indexInf, 0);
		vertexBuffer->Lock(0, 0, (void**)&vertex, 0);

		// 当たった三角形
		FVF	polyVertex[3];
		for (int i = 0; i < 3; i++) {
			polyVertex[i] = vertex[indexInf[*index * 3 + i]];
		}

		vertexBuffer->Unlock();
		indexBuffer->Unlock();

		D3DXVECTOR3 planeNor;
		D3DXVECTOR3 vecA = polyVertex[1].pos - polyVertex[0].pos;
		D3DXVECTOR3 vecB = polyVertex[2].pos - polyVertex[0].pos;

		D3DXVec3Cross(&planeNor, &vecA, &vecB);
		D3DXVec3Normalize(&planeNor, &planeNor);

		if (colPos) {
			*colPos = polyVertex[0].pos + u*vecA + v*vecB;
		}
		if (planeNor.y >= 0.9f) {
			*ground = true;
		}
		else {
			*ground = false;
		}
		SAFE_DELETE(index);
		return true;
	}
	SAFE_DELETE(index);
	return false;
}

// レイと壁（）
bool CollisionRayStage(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rayPos, LPD3DXMESH mesh, float *dist) {
	BOOL hit = false;		// 当たったかフラグ
	DWORD *index = new DWORD;			// 最短へのインデックス
	FLOAT u, v;			// 面のどの位置に当たったか
	pos += rayPos;
	D3DXIntersect(mesh, &pos, &move, &hit, index, &u, &v, dist, NULL, NULL);
	SAFE_DELETE(index);
	return hit;
}

// ---------------OBB------------------
// 分離軸に投影された軸成分から投影線分長を算出
FLOAT LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	FLOAT r1 = fabs(D3DXVec3Dot(Sep, e1));
	FLOAT r2 = fabs(D3DXVec3Dot(Sep, e2));
	FLOAT r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}
// OBB v.s. OBB
bool CollisionOBB(D3DXVECTOR3 pos1, D3DXVECTOR3 direct1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 direct2, D3DXVECTOR3 size2)
//bool ColOBBs(OBB &obb1, OBB &obb2)
{
	OBB obb1;
	OBB obb2;
	obb1.m_Pos = pos1;
	obb2.m_Pos = pos2;

	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	D3DXVECTOR3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W(0);
	D3DXVECTOR3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
	D3DXVECTOR3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
	D3DXVECTOR3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W(0);
	D3DXVECTOR3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
	D3DXVECTOR3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
	D3DXVECTOR3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

	// 分離軸 : Ae1
	FLOAT rA = D3DXVec3Length(&Ae1);
	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	FLOAT L = fabs(D3DXVec3Dot(&Interval, &NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

					  // 分離軸 : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be1);
	L = fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be2);
	L = fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}

