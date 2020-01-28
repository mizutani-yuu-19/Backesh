///////////////////////////////////////////////////
//
// �L�����N�^�[
//
//////////////////////////////////////////////////
#ifndef _PLAYER_H_
#define _PLAYER_H_
//----------------�C���N���[�h-------------------
#include "main.h"
#include "water_atk.h"
#include "game.h"
#include "scene_mgr.h"
#include "water_puddle.h"
#include "character.h"
//---------------�萔�}�N��------------


//-------------�N���X----------------
class CPlayer : public CCharacter
{
public:
	CPlayer(int characterType, int teamNo, int bucketType);
	~CPlayer();

	void Init();
	void Uninit();
	void Update();
	void Draw(int camNo);

	void				SkillUpdate();		// �K�E�Z�X�V
private:
	void				Operate();			// ����S��

	void				LockOn();			// ���b�N�I��			
	void				LockOnUpdate();		// ���b�N�I���A�b�v�f�[�g			
	void				LockOnDraw();		// ���b�N�I���`��
	D3DXVECTOR3			m_lockOnPos;		// ���b�N�I�����̓G�̈ʒu
	int					m_lockOnNo;			// ���b�N���Ă���L����	�P�O�̈ʂɃ`�[���i���o�[�A�P�̈ʂɃ����o�[�i���o�[

	int					m_skillTime;		// �X�L���g�p���̃J�E���g
};

#endif // !_PLAYER_H_