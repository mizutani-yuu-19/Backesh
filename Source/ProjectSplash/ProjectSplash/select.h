///////////////////////////////////////////////////
//
//		�Z���N�g�V�[���@(select.h)
//
//////////////////////////////////////////////////
#ifndef _SELECT_H_
#define _SELECT_H_
//=============================================
//				�C���N���[�h
//=============================================
#include "main.h"
#include "scene_mgr.h"
#include "fade.h"
#include "sprite.h"
#include "game.h"
#include "character.h"
#include <vector>
using namespace std;

//=============================================
//				�萔��`
//=============================================
#define MAX_SET_NUM (1 + MAX_TEAMMEMBER * 2)
#define MAX_VS_TYPE (4)
//=============================================
//				�񋓑�
//=============================================

// �摜
enum SELECT_TEXTURE
{
	//�o�b�N���
	TEX_BACK = 0,

	//�t�H���g
	TEX_SINGLE_FONT,
	TEX_MULTI_FONT,
	TEX_2P_FONT,
	TEX_3P_FONT,
	TEX_4P_FONT,
	TEX_2P_VS_FONT,
	TEX_3P_VS_FONT,
	TEX_4P_VS_FONT,

	//�g
	TEX_PICK,

	//�t�B�[��
	TEX_FIELD_ROOM,
	TEX_FIELD_STREET,

	//�L�����N�^
	TEX_CHARA_LOUCUST,
	TEX_CHARA_FIREFLY,
	TEX_CHARA_BETTLE,

	//�o�P�c
	TEX_BUCKET_VERTICAL,
	TEX_BUCKET_HORIZONTAL,

	//��
	TEX_NEXT,

	//�G�̐�
	TEX_ENEMYTEAM,
	TEX_1TEAM,
	TEX_2TEAM,
	TEX_3TEAM,

	//�v���C���[�ԍ�
	TEX_PLAYER1,
	TEX_PLAYER2,
	TEX_PLAYER3,
	TEX_PLAYER4,

	//�߂�
	TEX_RELEASE,

	//�ő�
	MAX_TEX,
};

//=============================================
//�@���[�h���
//=============================================
enum MODE
{
	MODE_SELECT = 0,
	MODE_SET_GAMEPARAM,
	MODE_SET_GAME,

	MAX_MODE,
};

//=============================================
//���[�h�^�C�v
//=============================================
enum MODE_TYPE
{
	TYPE_SINGLE = 0,
	TYPE_MULTI,

	MAX_MODE_TYPE,
};
//
//
//
enum PLAYER_NUM
{
	PLAYERNUM_2 = 0,
	PLAYERNUM_3,
	PLAYERNUM_4,
	MAX_PLAYERNUM,
};
//
//
//
enum VS_3P
{
	VS_1P_COM_COM = 0,
	VS_1P_2P_COM,
	VS_1P_2P_3P,

	MAX_3P_VS,
};
enum VS_4P
{
	VS_1P_COM_COM_COM = 0,
	VS_1P_2P_COM_COM,
	VS_1P_2P_3P_COM,
	VS_1P_2P_3P_4P,

	MAX_4P_VS,
};

//=============================================
//	�I�����
//=============================================
enum STATE
{
	STATE_OK = 0,
	STATE_YET,

	MAX_STATE,
};
//�Q�[�����I��
enum GAME_PARAM
{
	SELECT_STAGE = 0,
	SELECT_TEAMMEMBER,
	SELECT_USE_BUCKET,
	SELECT_NEXT,
	SELECT_BACK,
	MAX_SELECT_PARAM
};

//=============================================
//					�\����
//=============================================
typedef struct
{
	D3DXVECTOR2 pos;	//�ʒu
	D3DXVECTOR2 size;	//�傫��
	int divideX;		//�����i���j
	int divideY;		//�����i�c�j

}ST_BASICPARAM;
//=============================================
//					�N���X
//=============================================
class CSelect :public CBaseScene, public CScene
{
public:
	CSelect();
	~CSelect();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 m_pDevice;
	// �쐬�e�N�X�`��
	LPDIRECT3DTEXTURE9 m_pTexSplitMode[MAX_TEX];
	// �쐬�e�N�X�`���T�[�t�F�C�X
	LPDIRECT3DSURFACE9 m_pSurfaceSplitSelect;
	// ��ʂ̃o�b�t�@�[
	LPDIRECT3DSURFACE9 m_pBackBuffer;

	//�o�b�N��ʃe�N�X�`�����
	CSprite *m_pSpriteBack;
	CSprite *m_pSpritePick;

	CSprite *m_pSpriteModeTex[MAX_MODE];
	CSprite *m_pSpriteStage[MAX_STAGE_TYPE];
	CSprite *m_pSpritePlayerNum[MAX_PLAYERNUM];
	CSprite *m_pSprite2pVs;
	CSprite *m_pSprite3pVs[MAX_3P_VS];
	CSprite *m_pSprite4pVs[MAX_4P_VS];
	CSprite *m_pSpriteCharacter[MAX_TEAMMEMBER][MAX_CHARACTER_TYPE];
	CSprite *m_pSpriteBucket[MAX_TEAMMEMBER][MAX_BUCKET_TYPE];
	CSprite *m_pSpriteNext;
	CSprite *m_pSpriteEnemyNum[MAX_TEAM];
	CSprite *m_pSpritePlayerNo[MAX_TEAM];
	CSprite *m_pSpriteRelease;

	//�o�b�N��ʏ��
	ST_BASICPARAM m_backPrm;
	//���[�h�I����ʏ��
	ST_BASICPARAM m_modePrm[MAX_MODE];
	//�X�e�[�W���
	ST_BASICPARAM m_stagePrm[MAX_STAGE_TYPE];
	//�L�����N�^���
	ST_BASICPARAM m_characterPrm[MAX_TEAMMEMBER][MAX_CHARACTER_TYPE];
	//�o�P�c���
	ST_BASICPARAM m_bucketPrm[MAX_TEAMMEMBER][MAX_BUCKET_TYPE];
	//
	ST_BASICPARAM m_playerNumPrm[MAX_PLAYERNUM];
	//
	ST_BASICPARAM m_2pVsPrm;
	ST_BASICPARAM m_3pVsPrm[MAX_3P_VS];
	ST_BASICPARAM m_4pVsPrm[MAX_4P_VS];

	//�I��g���
	ST_BASICPARAM m_pickPrm;
	//���{�^�����
	ST_BASICPARAM m_nextPrm;

	//�߂�{�^�����
	ST_BASICPARAM m_releasePrm;
	//�G�̐��摜���
	ST_BASICPARAM m_enemyNumPrm[MAX_TEAM];

	//�v���C���[�ԍ�
	ST_BASICPARAM m_PlayerNoPrm[MAX_TEAM];

	//�Q�[�����i�[
	ST_GAMEPARAM m_gameParam;

	//�X�e�[�W�̐�
	int m_stageNo;

	//�L�����N�^o�^�C�v�ԍ�
	int m_charaNo[MAX_TEAMMEMBER * MAX_TEAM];

	//�o�P�c�^�C�v�ԍ�
	int m_bucketNo[MAX_TEAMMEMBER *MAX_TEAM];

	//���������������������[�h�I����񁁁���������������
	//
	bool m_setParamOK;
	//�Z���N�g��ʃ^�C�v
	int m_ModeType;
	//���[�h�ԍ� 
	int m_modeNo;
	//�t�H���g�̊g�嗦�J�E���g
	float m_scaleChange;
	//
	bool m_pushFlag;
	//
	int m_selectplayer;
	//
	int m_selectnow;
	//�G�̃`�[���̐�
	int m_enemyNum;
	//�G�̐��̑I��
	int m_selectEnemyNum;

	//////////////
	//
	int m_vsDivideNum;
	//
	int m_editVs;
	//
	int m_selectVsNow;

	//�������������������I����񁁁���������������
	//
	int m_sumChara;
	//
	int m_playerNum;
	//
	int m_playerNo;
	//�I��͈�
	int m_range;
	//�I�𒆈ʒu
	int m_pickNo;
	//�I���t���O
	int m_setStateFlag;
	//
	int m_rangeOld;
	//
	int m_setCharaFlag[MAX_TEAMMEMBER];
	//
	int m_setBucketFlag[MAX_TEAMMEMBER];
	//
	int m_checkParam;
	//�������������������L�����N�^�I����񁁁���������������

	//�L�����N�^�^�C�v
	int m_charaType;
	//�ҏW�ԍ�
	int m_editChara;

	//�������������������o�P�c�I����񁁁���������������

	//�o�P�c�̃^�C�v
	int m_bucketType;
	//�I��
	int m_editBucket;

	//========== �v���g�^�C�v ==========
	void InitSelectInf();

	void SelectMode();

	void SetGamePrm();
	void StagePrm();
	void CharacterPrm();
	void BucketPrm();
	void DrawModeSelect();

	void LoadTexture();

};

#endif // !_SELECT_H_
