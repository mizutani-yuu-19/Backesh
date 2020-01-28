///////////////////////////////////////////////////
//
// �Q�[���V�[��
//
//////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_
//----------------�C���N���[�h-------------------
#include "main.h"
#include "scene_mgr.h"

//=============================================
//			�� �� �� �`
//=============================================
#define MAX_MODEL		(10)				//�ő僂�f����
#define	MAX_TEAM		(4)					//�ő�`�[����
#define MAX_TEAMMEMBER	(3)					//�ő�`�[�����l��
#define GAME_TIME		(3.0f*3600)			//�Q�[���̎��ԂR��

#define PLAYER_COLOR1	(D3DCOLOR_ARGB(255, 0, 0, 255))
#define PLAYER_COLOR2	(D3DCOLOR_ARGB(255, 255, 0, 0))
#define PLAYER_COLOR3	(D3DCOLOR_ARGB(255, 0, 255, 0))
#define PLAYER_COLOR4	(D3DCOLOR_ARGB(255, 255, 255, 0))
//=============================================
//			 �� �� �� �� �`
//=============================================

//�@�L�����N�^�̎��
enum CHARACTER_TYPE
{
	CHARACTER_TYPE_LOCUST = 0,	// �o�b�^
	CHARACTER_TYPE_FIREFLY,		// �u
	CHARACTER_TYPE_BETTLE,		//�J�u�g���V

	MAX_CHARACTER_TYPE	//�L�����N�^��
};
//�X�e�[�W�̎��
enum STAGE_TYPE
{
	TYPE_ROOM = 0,	// ����
	TYPE_ROAD,		// �H�n

	MAX_STAGE_TYPE		//�X�e�[�W�̍ő吔
};
//�`�[���̎��
enum TEAM_TYPE
{
	TEAM_TYPE_P1 = 0,	//�v���C���[�P
	TEAM_TYPE_P2,		//�v���C���[�Q
	TEAM_TYPE_COM,		//�R���s���[�^�[

	MAX_TEAM_TYPE

};
//�o�P�c�̎��
enum BUCKET_TYPE
{
	BUCKET_VERTICAL = 0,
	BUCKET_HORIZONTAL,

	MAX_BUCKET_TYPE
};

//�X�e�b�v
typedef enum {
	STEP_STAGEVIEW,
	STEP_CHARACTERVIEW,
	STEP_COUNTDOWN,
	STEP_GAME,
	STEP_FINISH,
}EN_STEP;

//=============================================
//�@�\ �� �� �� ��
//=============================================
typedef struct
{
	int characterType[MAX_TEAMMEMBER*MAX_TEAM];	//�L�����N�^�[�^�C�v
	int bucketType[MAX_TEAMMEMBER*MAX_TEAM];	//�o�P�c�̃^�C�v
	int stageType;		//�X�e�[�W�^�C�v
	int teamNum;		//�`�[����
	int playerNum;		//�v���C���[��
}ST_GAMEPARAM;
//-------------�N���X----------------
class CGame :public CBaseScene
{
public:
	CGame();
	~CGame();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	static void SetGameParam(ST_GAMEPARAM);
	static ST_GAMEPARAM GetGameParam();
	static int	GetTime();

	static int m_step;
	static int GetStep() { return m_step; }
	static void SetStep(int step) { m_step = step; }
private:
	static ST_GAMEPARAM m_gameParam;

	static int m_gameTime;
};

#endif // !_GAME_H_