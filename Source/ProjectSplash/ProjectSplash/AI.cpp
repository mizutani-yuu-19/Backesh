///////////////////////////////////////////////////
//
// �L�����N�^�[
//
//////////////////////////////////////////////////
//----------------�C���N���[�h--------------
#include "ai.h"
#include "game.h"
#include "game_pad.h"
#include "camera.h"
#include "collision.h"
#include "input.h"
#include "water_puddle.h"
#include "debugproc.h"
#include "AI.h"
#include "player.h"
#include "stage.h"

//-------------�萔�}�N��--------------
#define VALUE_MOVE_AI		(2.0f)					//AI�ړ����x
#define MAX_ROTCOUNT		(6)						//��]��^����܂ł̍ő�t���[����
#define MAX_RESETCOUNT		(30)					//��]�����Z�b�g����J�E���g
#define MAX_ROTPATCOUNT		(240)					//��]������ς���܂ł̍ő�t���[����
#define SENSOR_DIST			(60.0f)					//�ǂƂ̏Փ˂����m���鋗��
#define SEARCH_RANGE		(400.0f)				//AI�̓G�T�[�`�͈�
#define MAX_SEARCHCOUNT		(0)						//�ŒZ�����ɂ���G�̒T�����X�V����J�E���g��
#define ATTACK_RANGE		(80.0f)				//�U�����s���͈�
#define JUMP_RANGE			(50.0f)					//�W�����v���s�������f���鋗��


//----------�O���[�o��-----------
// �R���X�g���N�^
CAi::CAi(int aiType, int teamNo, int bucketType) :CCharacter(aiType, teamNo, bucketType)
{
	//�U���p�^�[���̏�����
	m_atkType = 0;

	//������^����
	//AI�̌������O�`�S�̂T�i�K�Ƀ����_���Ɋ���U��
	m_intelligence = rand() % 5;

	//�ǂ�F��������������ς���J�E���g������
	m_wallTime = 0;

	//��̒n�_��ڎw�����߂̃t���O������
	m_markUP = false;

	//AI�ƓG�̍ŒZ�����ɃX�e�[�W��������ď��������Ă���
	m_pDistVector = D3DXVECTOR3(CStage::m_stage->GetStageSize().x, CStage::m_stage->GetStageSize().y, CStage::m_stage->GetStageSize().z);

	//�G�T���J�E���g������
	m_searchCount = MAX_SEARCHCOUNT;

	//�����_���T�[�`������
	m_randSearch = false;

	//���[�h������
	m_modeRotWall = false;

	//�㏸���t���O������
	m_upStairs = false;

}
void CAi::ChangeOccasion() {
	//�ǂ�F��������������ς���J�E���g������
	m_wallTime = 0;

	//��̒n�_��ڎw�����߂̃t���O������
	m_markUP = false;

	//AI�ƓG�̍ŒZ�����ɃX�e�[�W��������ď��������Ă���
	m_pDistVector = D3DXVECTOR3(CStage::m_stage->GetStageSize().x, CStage::m_stage->GetStageSize().y, CStage::m_stage->GetStageSize().z);

	//�G�T���J�E���g������
	m_searchCount = MAX_SEARCHCOUNT;

	//�ǂ�F��������������ς���J�E���g������
	m_wallTime = 0;

	//�W�����v�\�ȕǂƂ̍ŒZ����������
	m_wallDistJump = 0.0f;

	//��]���Z�b�g�J�E���g�̏�����
	m_resetCount = 0;

	//AI�ƓG�̊p�x������
	m_pRotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�평����
	m_markUpPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_markUpDistVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_upStairsPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_upStairsVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�^�[�Q�b�g�ԍ��֌W������
	m_targetNo = 0;
	m_prevTargetNo = 0;

	//���֌W������
	m_refugePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_refugeVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//��Փx
	difficulty = rand() % 4;

	//���[�h������
	m_modeRotWall = false;

	//�����_���T�[�`������
	m_randSearch = false;

	//�㏸���t���O������
	m_upStairs = false;
}

// �f�X�g���N�^
CAi::~CAi() {

}

// �ʒu�Ƃ�������
void CAi::Init() {
	CCharacter::Init();
	m_markUP = false;
}

void CAi::Uninit() {

}

void CAi::Update() {
	//�e�L�����N�^�[�̓����̐ݒ�
	if (CGame::GetStep() == STEP_GAME) {
		if (m_surviveFlag&&m_pickUpTime <= 0) {
			Operate();
		}
	}
	// �L�����N�^�[�̃A�b�v�f�[�g
	CCharacter::Update();
}

void CAi::Draw(int camNo) {
	CCharacter::Draw(camNo);
}


// ����
void CAi::Operate()
{
	D3DXMATRIX *y = new D3DXMATRIX;

	//���C���L�����N�^�[�ȊO

	//�Q�p�^�[���Œ��I���s��
	m_atkType = rand() % 2;



	//AI�̐����Ȃ�������
	if (m_waterVolume < 1.0f&&m_pickUpHandle == -1)
	{
		//�O�Ԗڂ̐����܂�̏��������l�Ƃ��Ĉ���
		//if (m_puddleDistMin==NULL) {
		m_puddlePos = CWaterPuddle::GetWaterPuddlePos(0);
		m_puddleDistvector = m_puddlePos - m_beforePos;
		m_puddleDistvectorMin = m_puddleDistvector;
		m_puddleDistMin = CWaterPuddle::m_waterPuddle[0];
		//}
		//�S�����܂�̐���������
		for (int i = 0; i < CWaterPuddle::m_waterPuddle.size(); i++) {
			m_puddlePos = CWaterPuddle::GetWaterPuddlePos(i);
			m_puddleDistvector = m_puddlePos - m_beforePos;
			//�����܂�܂ł̍ŒZ�����������莟��X�V
			if (D3DXVec3Length(&m_puddleDistvector) < D3DXVec3Length(&m_puddleDistvectorMin))		// ��r��D3DXVec3Length�ɕς�����ŒZ��_���悤�ɂȂ���
			{
				m_puddleDistvectorMin = m_puddleDistvector;
				m_puddleDistMin = CWaterPuddle::m_waterPuddle[i];
			}
		}

		//AI���琅���܂�܂ł̊p�x�����߂�
		//�I�u�W�F�N�g�������Ă悯�邽�߂Ɍ����̍X�V���s���Ă���Ƃ���
		//�����܂�Ƃ̌����X�V���s��Ȃ�
		if (!m_modeRotWall && !m_markUP)
		{
			m_rot.y = atan2f(m_puddleDistvectorMin.z, m_puddleDistvectorMin.x);
		}

		//�ړ��ʍX�V
		//�����̍X�V�Ɠ��l�Ɉړ��ʂ̍X�V������
		if (!m_modeRotWall && !m_markUP)
		{
			m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
			m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
		}

		//�����܂�̏�Ƀv���C���[��������
		if (m_puddleDistMin) {
			if (CollisionAABB(m_beforePos + D3DXVECTOR3(0, m_size.y*0.5f, 0), m_size, m_puddleDistMin->m_pos, m_puddleDistMin->m_size))
			{
				PickUpWater();
			}
		}
	}

	//AI�������������Ă�����
	else
	{
		m_puddleDistMin = NULL;

		//�G���W�擾
		//�G�̍��W���擾�A�i�[
		for (int i = 0; i < m_teamNum; i++)
		{
			// �����̃`�[�������o�[��������X���[
			if (m_teamNo == i)	continue;
			for (int j = 0; j < MAX_TEAMMEMBER; j++)
			{
				if (!CCharacter::GetCharacterSurvive(i, j))	continue;
				//�ꎞ�I�ɓG�̍��W���i�[
				m_pPosTmp = CCharacter::GetCharacterPos(i, j);

				//�G��AI�Ƃ̋���(�x�N�g��)�����߂�
				//�ꎞ�I�Ɋi�[�A�ŒZ������r�p
				m_pDistVectorTmp = m_pPosTmp - m_beforePos;

				//�ŒZ�����ɂ���G��T��
				if (D3DXVec3Length(&m_pDistVector) > D3DXVec3Length(&m_pDistVectorTmp))
				{
					m_pDistVector = m_pDistVectorTmp;
					//�ŒZ�����ɂ���G�̍��W���擾
					m_pPos = m_pPosTmp;
					m_targetNo = i * 10 + j;
					m_shiftPos = true;
				}
			}
		}

		//�^�[�Q�b�g�ԍ��ɕύX�����������A��ɍs���̂���߂�
		if (m_targetNo != m_prevTargetNo)
		{
			m_markUP = false;
			m_prevTargetNo = m_targetNo;
		}


		//AI�̌����ɉ����đ���v���C���[�̍��W���擾�����l�����炷
		//�����im_intelligence�j�̒l���傫���قǑ傫�Ȃ��ꂪ������
		/*if (!m_randSearch && !m_modeRotWall && !m_markUP && m_shiftPos)
		{
		switch (difficulty)
		{
		case 0:
		m_pPos.x += rand() % 20 - 10;
		m_pPos.z += rand() % 20 - 10;

		case 1:
		m_pPos.x += rand() % 40 - 20;
		m_pPos.z += rand() % 40 - 20;

		case 2:
		m_pPos.x += rand() % 60 - 30;
		m_pPos.z += rand() % 60 - 30;

		case 3:
		m_pPos.x += rand() % 100 - 50;
		m_pPos.z += rand() % 100 - 50;

		case 4:
		m_pPos.x += rand() % 200 - 100;
		m_pPos.z += rand() % 200 - 100;
		}

		m_shiftPos = false;
		}
		*/

		//�G��AI�Ƃ̋���(�x�N�g��)�����߂�
		m_pDistVector = m_pPos - m_beforePos;


		D3DXVECTOR3 vec1;
		D3DXVECTOR3 vec2;

		//���K��
		D3DXVec3Normalize(&vec1, &m_move);
		D3DXVec3Normalize(&vec2, &m_pDistVector);

		//AI����G�̊p�x�����߂�
		//AI�ƓG�̊p�x����Ɏ����Ă��炤
		m_pRotEnemy.y = acosf(D3DXVec3Dot(&vec1, &vec2));

		//�I�u�W�F�N�g�������Ă悯�邽�߂Ɍ����̍X�V�y��
		//��Ɍ��������߂̈ړ����s���Ă���Ƃ���
		//�G�Ƃ̌����X�V���s��Ȃ�
		if (!m_modeRotWall && !m_markUP)
		{
			m_rot.y = atan2f(m_pDistVector.z, m_pDistVector.x);
		}

		//�G���O���̔��~�ɓ����Ă��邩�A�������߂�������G�����m����
		if (m_markUP || m_pRotEnemy.y < D3DX_PI * 0.5f && D3DXVec3Length(&m_pDistVector) < SEARCH_RANGE)
		{
			//�����_���T�[�`�̃t���O������
			m_randSearch = false;

			//XZ���ꂼ���cos,sin��n���A�ړ��ʂ����߂�
			if (!m_modeRotWall)
			{
				m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
				m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
			}

			//�G�������̍������������ꏊ�ɂ�����
			//���K�i�̉���ڎw��
			if (m_pPos.y > m_beforePos.y + 60.0f && !m_modeRotWall && !m_upStairs)			//�W�����v�ʂ���̂U�O
			{
				//���ڎw���t���O�𗧂Ă�
				m_markUP = true;


				//��ɍs�����߂Ɉꎞ�I�ɖڎw�����W�i���������A�����W�O�j���i�[
				m_markUpPos = D3DXVECTOR3(0.0f, 0.0f, 800.0f);						//���ł����Ă���܂�	

				//AI����̋���(�x�N�g��)�����߂�
				m_markUpDistVector = m_markUpPos - m_beforePos;

				//AI����̊p�x�����߂�
				m_rot.y = atan2f(m_markUpDistVector.z, m_markUpDistVector.x);
			}

			if (m_pPos.y < m_beforePos.y + 60.0f)
			{
				m_markUP = false;
				m_upStairs = false;
			}

			//�ڕW�̍��W�܂ŗ�����
			if (D3DXVec3Length(&m_upStairsVector) < 10.0f && m_upStairs && !m_modeRotWall)
			{
				//���ڎw���t���O�����낷
				m_markUP = false;
				m_upStairs = false;
			}


			//���K�i�̉��ɗ�����
			if (m_markUP && D3DXVec3Length(&m_markUpDistVector) < 10.0f && !m_modeRotWall)
			{
				//�㏸���t���O�𗧂Ă�
				m_upStairs = true;

				//���K�i�̏�ɓ�������W���i�[
				m_upStairsPos = D3DXVECTOR3(0.0f, 0.0f, -800.0f);						//���Œu���Ă���܂�

				m_upStairsVector = m_upStairsPos - m_beforePos;

				m_rot.y = atan2f(m_upStairsVector.z, m_upStairsVector.x);
			}


			//AI���L�����N�^�[������ȏ�߂�������
			if (D3DXVec3Length(&m_pDistVector) < ATTACK_RANGE)
			{
				//�ړ����Ȃ�
				m_move.x = cosf(m_rot.y) * 0.0f;
				m_move.z = sinf(m_rot.y) * 0.0f;


				//AI�U��
				if (m_waterVolume >= 1.0f)
				{
					if (!m_water) {
						D3DXVECTOR3 angle, pos;
						pos = m_beforePos;
						pos.y += 10.0f;
						m_water = new CWaterAtk(pos, m_rot.y*-1.0f + D3DX_PI*0.5f, m_waterVolume, m_bucketType);
						m_waterVolume = 0.0f;
					}
				}
			}

			//����������
			//�����̍X�V�Ɠ��l�Ɉړ��ʂ̍X�V������
			else if (!m_modeRotWall /*&& m_life >= 50*/)
			{
				//�ړ��ʍX�V
				m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
				m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
			}

			/*if (m_life < 50)
			{
			m_refugePos = D3DXVECTOR3(800.0f, 0.0f, 0.0f);

			m_refugeVector = m_refugePos - m_beforePos;

			m_rot.y = atan2f(m_refugeVector.z, m_refugeVector.x);

			//�ړ��ʍX�V
			m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
			m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
			}
			*/
		}



		//�G��T���Č�����Ȃ�������A�����_���ȍ��W�Ɍ����킹��
		//���̍��W�ɍs���܂łɓG������������T�[�`������if���ɓ���
		//���s������A�܂������_�����W��^����̌J��Ԃ�
		else if (!m_randSearch)
		{
			//�}�b�v��̃����_�����W��^����
			m_pRandPos = CStage::m_missingPoint[rand() % MAX_MISSING_POINT];

			//�t���O�����Ă�
			m_randSearch = true;

		}

		else if (m_randSearch)
		{
			D3DXVECTOR3 temp = m_pRandPos - m_beforePos;
			if (D3DXVec3Length(&temp) < 50.0f)
			{
				m_randSearch = false;
			}
		}
		if (m_randSearch && !m_modeRotWall && !m_markUP) {
			//�����_�����W��AI�Ƃ̋���(�x�N�g��)�����߂�
			m_pDistVector = m_pRandPos - m_beforePos;


			//AI���烉���_�����W�̊p�x�����߂�
			m_rot.y = atan2f(m_pDistVector.z, m_pDistVector.x);

			//�ړ��ʂ�^����
			//XZ���ꂼ���cos,sin��n��
			m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
			m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
		}
	}

	//�W�����v�ŉz������I�u�W�F�N�g�̍ŒZ�������o��
	if (CollisionRayStage(m_beforePos, m_move, D3DXVECTOR3(0.0f, 50.0f, 0.0f), CStage::m_stage->m_pColStageMeshModel, &m_wallDistJump))
	{
	}
	else
	{
		m_wallDistJump = 9999.0f;
	}

	//�w�̒Ⴂ��Q�����W�����v�Ŕ�щz����
	if (m_wallDistFront <= JUMP_RANGE && m_wallDistJump > JUMP_RANGE && !m_jumpFlag)
	{
		m_move += D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		m_jumpFlag = true;
	}


	//�O���ɃI�u�W�F�N�g����������I�u�W�F�N�g���悯��(��������])
	if (m_wallDistFront < SENSOR_DIST && m_wallTime <= 0 && m_wallDistJump < JUMP_RANGE)
	{
		m_modeRotWall = true;

		if (m_rotCount <= 0)
		{
			//��]�����𒊑I
			m_rotPattern = rand() % 2;
			m_rotCount = MAX_ROTPATCOUNT;
		}

		//�E�����A�����_�������ɉ�]
		if (m_rotPattern == 0)
		{
			m_rot.y += D3DX_PI / 12.0f;
		}

		else
		{
			m_rot.y -= D3DX_PI / 12.0f;
		}
		m_wallTime = MAX_ROTCOUNT;
		m_resetCount = MAX_RESETCOUNT;

		//�ړ��ʍX�V
		m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
		m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;

	}



	//�Ȃ���t���O������
	else if (m_wallDistFront >= SENSOR_DIST && m_modeRotWall == true && m_resetCount < 0)
	{
		m_modeRotWall = false;
	}

	//�ǔF���J�E���g����
	m_wallTime--;

	//��]���Z�b�g�J�E���g����
	m_resetCount--;

	//��]�����ύX�J�E���g
	m_rotCount--;

	if (m_searchCount <= 0)
	{
		//�T�b�o�߂��ƂɓG��AI�̋������ő�l�ɐݒ肵�Ȃ���
		m_pDistVector = D3DXVECTOR3(CStage::m_stage->GetStageSize().x, CStage::m_stage->GetStageSize().y, CStage::m_stage->GetStageSize().z);
		m_puddleDistvectorMin = D3DXVECTOR3(9999, 9999, 9999);
		m_searchCount = MAX_SEARCHCOUNT;
	}

	//�ŒZ�����ɂ���G�T���J�E���g����
	m_searchCount--;

	// �����������
	m_rot.y += D3DX_PI*-0.5f;
	delete y;
}
