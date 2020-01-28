///////////////////////////////////////////////////
//
// �L�����N�^�[
//
//////////////////////////////////////////////////
#ifndef _AI_H_
#define _AI_H_
//----------------�C���N���[�h-------------------
#include "main.h"
#include "water_atk.h"
#include "game.h"
#include "scene_mgr.h"
#include "water_puddle.h"
#include "character.h"
//---------------�萔�}�N��------------

//-------------�N���X----------------
class CAi :public CCharacter
{
public:
	CAi(int aiType, int teamNo, int bucketType);
	~CAi();

	void Init();
	void Uninit();
	void Update();
	void Draw(int camNo);

	void ChangeOccasion();
private:

	int					difficulty;					//��Փx

	int					m_atkType;					//�U���p�^�[���Q��	
	int					m_intelligence;				//AI�̌���
	int					m_rotPattern;				//�ǂɓ����������̉�]�������I�p
	int					m_wallTime;					//�ǂ�F����������]�����Ɋp�x��^����J�E���g
	int					m_rotCount;					//�ǂ�F��������������ς���J�E���g
	int					m_resetCount;				//��]���Z�b�g�J�E���g
	int					m_searchCount;				//�ŒZ�����ɂ���G�T���J�E���g
	int					m_targetNo;					//�_���Ă���G�ԍ�
	int					m_prevTargetNo;				//�ЂƂO�ɑ_���Ă����G�ԍ�
	float				m_wallDistJump;				//�W�����v�\�ȕǂƂ̍ŒZ����

	D3DXVECTOR3			m_pPos;						//�G���W�i�[�p
	D3DXVECTOR3			m_pRandPos;						//�G���W�i�[�p
	D3DXVECTOR3			m_pPosTmp;					//�G���W�i�[�p�i�ꎞ�I�j
	D3DXVECTOR3			m_pDistVector;				//AI�ƓG�̋����i�x�N�g���j
	D3DXVECTOR3			m_pDistVectorRand;			//AI�ƃ����_�����W�̋���
	D3DXVECTOR3			m_pDistVectorTmp;			//AI�ƓG�̋����i�x�N�g���j���ꎞ�I�Ɋi�[
	D3DXVECTOR3			m_pRotEnemy;				//AI�ƓG�̊p�x����������
	D3DXVECTOR3			m_markUpPos;				//��ɍs�����߂ɖڎw�����W
	D3DXVECTOR3			m_markUpDistVector;			//��ɍs�����߂ɖڎw�����W�Ƃ̋���
	D3DXVECTOR3			m_upStairsPos;				//��i�ŏI�n�_�j���W
	D3DXVECTOR3			m_upStairsVector;			//��i�ŏI�n�_�j�Ƃ̋���
	D3DXVECTOR3			m_refugePos;				//HP�����Ă����Ԃł̔����W
	D3DXVECTOR3			m_refugeVector;				//HP�����Ă����Ԃł̔����W�Ƃ̋���

	D3DXVECTOR3			m_puddlePos;				//�����܂�̍��W�i�[�p
	D3DXVECTOR3			m_puddleDistvector;			//AI�Ɛ����܂�̋����i�[�p
	D3DXVECTOR3			m_puddleDistvectorMin;		//AI�Ɛ����܂�̍ŒZ����
	CWaterPuddle*		m_puddleDistMin;			// ��ԋ߂������܂�̃A�h���X

	bool				m_markUP;					//��̒n�_��ڎw�����߂̍��W�Ɍ������i�K�i�Ƃ���Ƃ��j
	bool				m_upStairs;					//���ۂɏ�ɍs���Ă���
	bool				m_randSearch;				//�G���T�[�`�͈͊O�̎��A�����_�����W��T���t���O
	bool				m_shiftPos;					//�G�̍��W���Փx�ɉ����Ă��炷




	D3DXMATRIX			m_mtxWorld;					// ���[���h�}�g���N�X



	void DeathAi();									// �L�������S

	void Operate();									//����S��

	bool				m_modeRotWall;				//���݂̃��[�h

};

#endif // !_AI_H_