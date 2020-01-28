///////////////////////////////////////////////////
//
// �L�����N�^�[
//
//////////////////////////////////////////////////
#ifndef _CHARACTER_H_
#define _CHARACTER_H_
//----------------�C���N���[�h-------------------
#include "main.h"
#include "water_atk.h"
#include "game.h"
#include "scene_mgr.h"
#include "water_puddle.h"
#include "MeshObj.h"
#include "model.h"

//---------------�萔�}�N��------------
// ���f���t�@�C����
#define	MODEL_CHARACTER_LOUCUST		("data/MODEL/loucust")	//�o�b�^
#define	MODEL_CHARACTER_FIREFLY		("data/MODEL/firefly")	//�u
#define	MODEL_CHARACTER_BEETLE		("data/MODEL/beetle")	//�J�u�g���V

//------------�񋓑̒�`---------------

// �A�j���[�V������
enum {
	ANIM_PICKUP,		// ����������
	ANIM_SIDESTEP,		// ������
	ANIM_SPLASH,		// �X�v���b�V��
	ANIM_JUMP,			// �W�����v
	ANIM_RUN,			// ����
	ANIM_WAIT,			// �ҋ@
	ANIM_MAX
};

//-------------�N���X----------------
class CCharacter : public CMeshObj
{
public:
	CCharacter(int characterType, int teamNo, int bucketType);
	~CCharacter();

	virtual void Init();
	void Uninit();
	virtual void Update();
	virtual void Draw(int camNo);

	static void AllUpdate();
	static void AllDraw(int camNo);

	static int m_killAllCnt[MAX_TEAM];				// �`�[���̃g�[�^���L���J�E���g
	static int m_deathAllCnt[MAX_TEAM];				// �`�[���̃g�[�^���f�X�J�E���g


	static int			m_teamNum;					// �`�[����
	static CCharacter*	m_character[MAX_TEAM][MAX_TEAMMEMBER];		//�S�L�����N�^�[�̔z��

	static D3DXVECTOR3	GetPlayerPos(int teamNo);					// �v���C���[�̍��W��Ԃ�
	static D3DXVECTOR3	GetPlayerMove(int teamNo);					// �v���C���[�̈ړ��ʂ�Ԃ�
	static float		GetPlayerRot(int teamNo);					// �v���C���[�̉�]����
	static float		GetPlayerWaterVol(int teamNo);				// �v���C���[�̐��c��
	static float		GetPlayerLife(int teamNo);					// �v���C���[�̐��c��
	static D3DXVECTOR3	GetPlayerKillerPos(int teamNo);				// �v���C���[���E������̈ʒu
	static bool			GetPlayerSurvive(int teamNo);				// �v���C���[�������Ă�
	static float		GetPlayerSkillGauge(int teamNo);				// �v���C���[�̃X�L���Q�[�W

	static CCharacter*	changeFrom;									// �v���C���[�`�F���W��
	static CCharacter*	changeTo;									// �v���C���[�`�F���W��
	static int			m_contentPos[MAX_TEAM][MAX_TEAMMEMBER];				// �z��������̒��g���ǂ��ɍs������
	static void			ChangePlayer();								// �L�����`�F���W
	static int			GetCharacterContentPos(int teamNo, int memberNo) { return m_contentPos[teamNo][memberNo]; }
	//	static	bool		GetCharacterInvincible(int teamNo, int memberNo) { return m_character[teamNo][memberNo]; }				// �v���C���[�̃X�L���Q�[�W

		// �L�����N�^�[�쐬
		// �����P�L�����̎�ނ̔z��̐擪�A�h���X
		// �����Q�v���C���[�̐l��
		// �����R�`�[���̐�
	static void CreateCharacter(int *type, int, int, int *bucketType);
	static void LoadCharacterModel();					//���f���ƃe�N�X�`�������[�h
	static void ReleaseCharacterModel();					//���f���ƃe�N�X�`���������[�X

	// �L�����N�^�[�̃^�C�v��Ԃ�
	static int GetCharacterType(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_type; };
	static float GetCharacterLife(int teamNo, int memberNo) {
		if (m_character[teamNo][memberNo]->m_life < 0.0f)	return 0.0f;
		return (float)(m_character[teamNo][memberNo]->m_life / (float)m_character[teamNo][memberNo]->m_maxLife);
	};
	static bool GetCharacterSurvive(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_surviveFlag; }
	static int GetCharacterInvincible(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_invincibleTime; }
	static CWaterAtk* GetCharacterWater(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_water; }
	//static bool GetCharacterSkillFlag(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_skillFlag; }
	// �L�����N�^�[�̈ʒu��Ԃ�
	D3DXVECTOR3 GetCharacterPos(int teamNo, int memberNo) { return m_character[teamNo][memberNo]->m_afterPos; };
	// �L������Ԃ�
	static int GetKillCount(int teamNo) { return m_killAllCnt[teamNo]; }
	static void CharacterKill(int teamNo, int memberNo, int killerNo) { m_character[teamNo][memberNo]->Kill(killerNo); }
	void Kill(int killerNo);

	static int m_winner;
	static void ResultFunc();
	static int GetWinner();
protected:
	D3DXVECTOR3			m_beforePos;				//���݂̈ʒu
	D3DXVECTOR3			m_afterPos;					//���݂̈ʒu
	float				m_moveSpeed;				//�ړ��X�s�[�h
	float				m_maxMoveSpeed;				//�ړ��X�s�[�h
	D3DXVECTOR3			m_rot;						//���݂̌���
	D3DXVECTOR3			m_size;						//�傫��
	int					m_teamNo;					//�`�[���i���o�[
	int					m_type;						//�L�����N�^�[�̃^�C�v

	int					m_No;						//�`�[�����ł̔ԍ�
	int					m_life;						//���̗̑�
	int					m_maxLife;					//�ő�̗�
	bool				m_jumpFlag;					//�W�����v�t���O
	bool				m_playerFlag;				//�v���C���[�t���O

	bool				m_surviveFlag;				//�����t���O
	int					m_killer;					//�N�ɎE���ꂽ
	int					m_respawnTime;				//�����Ԃ鎞��

	float				m_avoidCoolTime;			// ����N�[���^�C��
	float				m_invincibleTime;			// ���G����

	int m_killCnt;									// �����̃L���J�E���g
	int m_deathCnt;									// �����̃f�X�J�E���g

	D3DXVECTOR3			m_move;						//�ړ���
	D3DXVECTOR3			m_accel;					//������

	float				m_floorDist;				// ���܂ł̋���
	float				m_wallDistFront;			// �ǂ܂ł̋���

	D3DXMATRIX			m_mtxWorld;					//���[���h�}�g���N�X

	bool				m_sideStepFlag;				// �������t���O
	bool				m_splashFlag;				// �������t���O

	// ���֌W
	float				m_waterVolume;		// ���̗�
	float				m_pickUpVolume;		// ���������̗�
	int					m_pickUpTime;		// �����������c�莞��
	int					m_pickUpHandle;		// �����������܂�̃n���h��
	CWaterAtk			*m_water;			// �o�P�c�N���X
	int					m_bucketType;		// �o�P�c�̃^�C�v
	void				PickUpWater();		// �����������֐�
	static MODEL		m_bucketModel[2];	// �o�P�c�̃��f��

	// �K�E�Z
	float				m_skillGauge;		// �K�E�Z�̃Q�[�W�ő�P�D�O��
	bool				m_skillFlag;		// �K�E�Z�g�p��
	float				m_skillAccumulate;	// �Q�[�W�񕜗�

	static LPDIRECT3DTEXTURE9		m_pD3DTextureLockOn;		// �e�N�X�`���ւ̃|�C���^
public:
	void DamageLife();							//��e�_���[�W
	bool RespawnCharacter();					//���X�|�[������
	void KillCountAdd();						// �L���J�E���g�𑝂₷�B�|�����Ƃ��ɌĂ�ł�
	void DeathCountAdd();						// �f�X�J�E���g�𑝂₷�B���񂾂Ƃ��ɌĂ�ł�
private:
	D3DXVECTOR3 m_respawnPos;

	int m_healCnt;								// �񕜂���Ԋu
	float m_walkSECnt;							// �������o���J�E���g

	void UpdatePickUp();
};

#endif // !_CHARACTER_H_