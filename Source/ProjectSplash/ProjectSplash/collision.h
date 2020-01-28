// �Փ˔���
#pragma once

#include "main.h"

//---------�\���̒�`------------


// ��`���m�̏Փ˔���
bool CollisionRect(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh);
// �~���m�̏Փ˔���
bool CollisionCircle(float Ax, float Ay, float Ar, float Bx, float By, float Br);
// ���Ɛ��̏Փ˔���
bool CollisionLineLine(float A1x, float A1y, float A2x, float A2y, float B1x, float B1y, float B2x, float B2y);
// ��`�Ɛ�
bool CollisionRectLine(float left, float top, float right, float bottom, float x1, float y1, float x2, float y2);
// �~�Ɛ�
bool CollisionCircleLine(float px, float py, float r, float ax, float ay, float bx, float by);
// ���Ɛ��̂����蔻��A��܈�������_
bool CrossLine(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, D3DXVECTOR2 pos3, D3DXVECTOR2 pos4, D3DXVECTOR2 *CrossP);
// AABB�Փ˔���
bool CollisionAABB(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2);
// ���C�ƕǂ̓����蔻��
//�P�F���ݒn�̃A�h���X(In&Out)
//�Q�F�ړ�����(In)
//�R�F�n�`���f���̃��b�V��(In)
//�S�F�ǂƂ̋������i�[����A�h���X(Out)
bool CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 move, LPD3DXMESH mesh, float *dist);
// ���C�ƒn�ʂ̓����蔻��
//�P�F���ݒn�̃A�h���X(In&Out)
//�Q�F�n�`���f���̃��b�V��(In)
//�R�F�ǂƂ̋������i�[����A�h���X(Out)
bool CollisionStage(D3DXVECTOR3 *pos, LPD3DXMESH mesh, float *dist);

// ���C�ƕǁi���p�[�e�B�N���p�j
//�P�F���ݒn(In)
//�Q�F�ړ�����
//�R�F�n�`���f���̃��b�V��(In)
//�S�F���������ꏊ���i�[����A�h���X(Out)
//�T�F���������ꏊ���n�ʂ�(Out)
bool CollisionWaterStage(D3DXVECTOR3 pos, D3DXVECTOR3 move, LPD3DXMESH mesh, D3DXVECTOR3 *colPos, bool *ground);

// ���C�ƕǁi���p�[�e�B�N���p�j
//�P�F���ݒn(In)
//�Q�F�ړ�����
//�R�F�������烌�C���΂��ʒu�܂ł̈ړ���(In)
//�S�F�n�`���f���̃��b�V��(In)
//�T�F����(Out)
bool CollisionRayStage(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rayPos, LPD3DXMESH mesh, float *dist);

// �ʒu�A�����A�傫��
bool CollisionOBB(D3DXVECTOR3 pos1, D3DXVECTOR3 direct1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 direct2, D3DXVECTOR3 size2);

class OBB
{
protected:

public:
	D3DXVECTOR3 m_Pos;              // �ʒu
	D3DXVECTOR3 m_NormaDirect[3];   // �����x�N�g��
	FLOAT m_fLength[3];             // �e�������̒���
	D3DXVECTOR3 GetDirect(int elem) { return m_NormaDirect[elem]; }   // �w�莲�ԍ��̕����x�N�g�����擾
	FLOAT GetLen_W(int elem) { return m_fLength[elem]; }          // �w�莲�����̒������擾
	D3DXVECTOR3 GetPos_W() { return m_Pos; }             // �ʒu���擾
};