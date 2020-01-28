// �Փ˔���
#include "collision.h"
#include <stdlib.h>
#include <math.h>

// ��`���m�̏Փ˔���
bool CollisionRect(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh) {
	return (Ax <= Bx + Bw && Bx <= Ax + Aw && Ay <= By + Bh && By <= Ay + Ah);
}
// �~���m�̏Փ˔���
bool CollisionCircle(float Ax, float Ay, float Ar, float Bx, float By, float Br) {
	float dx = Ax - Bx;
	float dy = Ay - By;
	float r = Ar + Br;
	return (dx*dx + dy*dy <= r*r);
}
// ���Ɛ�
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
// ��`�Ɛ�
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
// �~�Ɛ�
bool CollisionCircleLine(float px, float py, float r, float ax, float ay, float bx, float by) {
	if (r < 0)	return false;
	// AB�̊p�x-AP�̊p�x=��
	float sita = atan2(ay - by, ax - bx) - atan2(ay - py, ax - px);
	if (sita < 0)	sita*-1.0f;
	// AB�̒���
	float lenAB = sqrt((ax - bx)*(ax - bx) + (ay - by)*(ay - by));
	if (lenAB < 0)	lenAB *= -1.0f;	// ��Βl�ϊ�
	// AP�̒���
	float lenAP = sqrt((ax - px)*(ax - px) + (ay - py)*(ay - py));
	if (lenAP < 0)	lenAP *= -1.0f;	// ��Βl�ϊ�
	// ����AX
	float lenAX = lenAP*cos(sita);
	//����AP��P�̍ŒZ����
	float shortDist = 0;
	if (lenAX < 0) {
		//AX�����Ȃ� AP���~�̒��S�܂ł̍ŒZ����
		//shortDist = lenAP;
		shortDist = 1000;
	}
	else if (lenAX > lenAB) {
		//AX��AP���������ꍇ�́ABP���~�̒��S�܂ł̍ŒZ����
		//shortDist = lenAB;
		shortDist = 1000;
	}
	else {
		//P��AB��ɂ���̂ŁAXP���ŒZ����
		float lenXP = lenAP*sin(sita);
		if (lenXP < 0)	lenXP *= -1.0f;
		shortDist = lenXP;
	}
	// �ŒZ���������a���Z�����
	if (shortDist < r) {
		return true;
	}
	return false;
}

// ���Ɛ��̂����蔻��A��܈����͌�_
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

//AABB�Փ˔���
bool CollisionAABB(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
{
	D3DXVECTOR3 min1;		//��r�ΏۂP�̍ŏ����W
	D3DXVECTOR3 max2;		//��r�ΏۂQ�̍ő���W
	min1 = pos1 - size1 * 0.5f;
	max2 = pos2 + size2 * 0.5f;

	//�P��X�ŏ����W�@���@�Q��Z�ő���W�̏ꍇ�Փ˂��Ă��Ȃ�
	if (min1.x > max2.x) return false;

	D3DXVECTOR3 max1;		//��r�ΏۂP�̍ő���W
	D3DXVECTOR3 min2;		//��r�ΏۂQ�̍ŏ����W
	max1 = pos1 + size1 * 0.5f;
	min2 = pos2 - size2 * 0.5f;

	//�P��X�ő���W�@���@�Q��X�ŏ����W�̏ꍇ�Փ˂��Ă��Ȃ�
	if (max1.x < min2.x) return false;
	//�P��Y�ŏ����W�@���@�Q��Y�ő���W�̏ꍇ�Փ˂��Ă��Ȃ�
	if (min1.y > max2.y) return false;
	//�P��Y�ő���W�@���@�Q��Y�ŏ����W�̏ꍇ�Փ˂��Ă��Ȃ�
	if (max1.y < min2.y) return false;
	//�P��Z�ŏ����W�@���@�Q��Z�ő���W�̏ꍇ�Փ˂��Ă��Ȃ�
	if (min1.z > max2.z) return false;
	//�P��Z�ő���W�@���@�Q��Z�ŏ����W�̏ꍇ�Փ˂��Ă��Ȃ�
	if (max1.z < min2.z) return false;

	//��L�̂ǂ̃p�^�[���ɂ�����Ȃ�������Փ�
	return true;
}


// ���C�ƕǂ̓����蔻��
bool CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 move, LPD3DXMESH mesh, float *dist) {
	if (move == D3DXVECTOR3(0.0f, 0.0f, 0.0f))	return false;
	BOOL hit = false;		// �����������t���O
	DWORD *index = new DWORD;			// �ŒZ�ւ̃C���f�b�N�X
	FLOAT u, v;			// �ʂ̂ǂ̈ʒu�ɓ���������
	//D3DXVECTOR3 colP;	// �Փ˃|�C���g
	float _dist;
	D3DXVECTOR3 _move;
	D3DXVec3Normalize(&_move, &move);
	D3DXVECTOR3 beforePos = *pos;// +_move * 10.0f;
	D3DXVECTOR3 afterPos = *pos + move;
	beforePos.y += 10.0f;
	D3DXVECTOR3 tempMove = move;
	tempMove.y = 0.0f;
	afterPos.y += 10.0f;
	// �����蔻��
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

		// ���������O�p�`
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

		// ���ʍ쐬
		D3DXPLANE	plane;
		D3DXPlaneFromPoints(&plane, &polyVertex[0].pos, &polyVertex[1].pos, &polyVertex[2].pos);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	�ړ��O��̍��W���|���S��������ł��邩���肵�A�͂���ł���Γ��������Ƃ݂Ȃ��A���ׂ菈�����s��


		//	�ړ���A�O�̈ʒu�����ʂ��͂��ނ��`�F�b�N
		float ans[2];	//	���w�̕��ʂƒ��_�Ɋւ�������𗘗p����F���ʂ̕\�ʂɂ���Ȃ琳�F���ʂȂ畉�ɂȂ�
		ans[0] = (plane.a * afterPos.x) + (plane.b * afterPos.y) + (plane.c * afterPos.z) + plane.d;
		ans[1] = (plane.a * beforePos.x) + (plane.b * beforePos.y) + (plane.c * beforePos.z) + plane.d;

		//	��̒l�̐��������΂Ȃ�΁A���ʂ��͂���->�������Ă���̂ł��ׂ菈�����s��
		if (ans[0] * ans[1] < 0.0f) {
			//	�ړ���̍��W����|���S���@�������Ƀ��C���΂��A�|���S���Ƃ̌�_�����߂�
			D3DXIntersect(mesh, &afterPos, &planeNor, &hit, index, NULL, NULL, &_dist, NULL, NULL);
			// Y����]�s��
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
			//	�������Ă����ꍇ�́A���C�\�����̏I�_�ʒu���X�V����
			if (hit&&_dist < 10.0f)
			{
				//	���ׂ��̍��W�|���S���̕\�ʂ�菭�����߂ɖ߂�
				afterPos = afterPos + planeNor*(_dist + 1.0f);

				//	���ׂĂ̏��������񂾍��W���Z�b�g
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

// ���C�ƒn�ʂ̓����蔻��
bool CollisionStage(D3DXVECTOR3 *pos, LPD3DXMESH mesh, float *dist) {
	BOOL hit = false;		// �����������t���O
	DWORD *index = new DWORD;			// �ŒZ�ւ̃C���f�b�N�X
	FLOAT u, v;			// �ʂ̂ǂ̈ʒu�ɓ���������
	D3DXVECTOR3 colP;	// �Փ˃|�C���g
	// ���K��
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	D3DXVECTOR3 beforePos = *pos;
	beforePos.y += 10.0f;
	// �����蔻��
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

		// ���������O�p�`
		FVF	polyVertex[3];
		for (int i = 0; i < 3; i++) {
			polyVertex[i] = vertex[indexInf[*index * 3 + i]];
		}

		vertexBuffer->Unlock();
		indexBuffer->Unlock();

		// �Ԃ�������߂荞�ݗʕ��߂�

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

// ���C�ƕǁi���p�[�e�B�N���p�j
bool CollisionWaterStage(D3DXVECTOR3 pos, D3DXVECTOR3 move, LPD3DXMESH mesh, D3DXVECTOR3 *colPos, bool *ground) {
	BOOL hit = false;		// �����������t���O
	DWORD *index = new DWORD;			// �ŒZ�ւ̃C���f�b�N�X
	FLOAT u, v;			// �ʂ̂ǂ̈ʒu�ɓ���������
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

		// ���������O�p�`
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

// ���C�ƕǁi�j
bool CollisionRayStage(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rayPos, LPD3DXMESH mesh, float *dist) {
	BOOL hit = false;		// �����������t���O
	DWORD *index = new DWORD;			// �ŒZ�ւ̃C���f�b�N�X
	FLOAT u, v;			// �ʂ̂ǂ̈ʒu�ɓ���������
	pos += rayPos;
	D3DXIntersect(mesh, &pos, &move, &hit, index, &u, &v, dist, NULL, NULL);
	SAFE_DELETE(index);
	return hit;
}

// ---------------OBB------------------
// �������ɓ��e���ꂽ���������瓊�e���������Z�o
FLOAT LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
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

	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	D3DXVECTOR3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W(0);
	D3DXVECTOR3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
	D3DXVECTOR3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
	D3DXVECTOR3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W(0);
	D3DXVECTOR3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
	D3DXVECTOR3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
	D3DXVECTOR3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

	// ������ : Ae1
	FLOAT rA = D3DXVec3Length(&Ae1);
	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	FLOAT L = fabs(D3DXVec3Dot(&Interval, &NAe1));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�

					  // ������ : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (L > rA + rB)
		return false;

	// ������ : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (L > rA + rB)
		return false;

	// ������ : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be1);
	L = fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (L > rA + rB)
		return false;

	// ������ : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be2);
	L = fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (L > rA + rB)
		return false;

	// ������ : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be3);
	L = fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (L > rA + rB)
		return false;

	// ������ : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}

