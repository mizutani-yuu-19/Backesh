///////////////////////////////////////////////////
//
// �^�C�g��
//
//////////////////////////////////////////////////
#ifndef _TITLE_H_
#define _TITLE_H_
//----------------�C���N���[�h-------------------
#include "main.h"
#include "scene_mgr.h"
#include "sprite.h"
#include "fade.h"

//�񋓑�
enum TITLE_TEXTURE	//�^�C�g���Ɏg���摜�̃i���o�����O
{
	NUM_BACK = 0,	//�o�b�N���
	NUM_TEAMLOGO,	//�`�[�����S�}�[�N
	NUM_STAFF,		//�X�^�b�t
	NUM_TITLEBACK,	//�^�C�g���o�b�N���
	NUM_GAMELOGO,	//�Q�[�����S
	NUM_SPLASH,		//�X�v���b�V��
	NUM_BUCKET,		//�o�P�c
	NUM_BUTTON,		//�{�^��

	MAX_NUM_TEXTURE,//�e�N�X�`���̍ő吔
};
//
typedef struct
{
	D3DXVECTOR2 pos;
	float sizeX;
	float sizeY;

}BASIC_PARAM;

//-------------�N���X----------------
class CTitle : public CBaseScene
{
public:

	CTitle();
	~CTitle();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:

	//�o�b�N�O���E���h���
	BASIC_PARAM m_backGroundParam;
	//�^�C�g���o�b�N
	BASIC_PARAM m_titleBackParam;
	//�`�[�����S���
	BASIC_PARAM m_teamLogoParam;
	//�X�^�b�t�摜���
	BASIC_PARAM m_staffParam;
	//�Q�[�����S���
	BASIC_PARAM m_gameLogoParam;
	//�X�v���b�V���G�t�F�N�g���
	BASIC_PARAM m_splashParam;
	//�o�P�c���
	BASIC_PARAM m_BucketParam;
	//�{�^���ݒ���
	BASIC_PARAM m_buttonParam;

	//�t�F�[�h�҂�����
	int m_fadeCount;
	//�؂�ւ��ԍ�
	int m_switchNo;

	//�J���[�ݒ�
	int m_alpha;

	//�{�^���J�E���g
	int m_buttomCount;
	//�{�^���̃X�C�b�`
	bool m_switchButton;
	//������Ă�����
	bool pushflag;
	//�o�P�c
	bool m_bucketSet;
	float m_bucketMove;
	//�X�v���b�V��
	//�Q�[�����S
	//�Q�[�����S�̕\�����I�������
	bool m_gamelogoflag;
	//
	D3DXVECTOR2 m_gamelogoScaleCnt;
	//
	int m_gamelogNo;
	int m_gamelogoNum;
	//
	float m_gamelogoMove;
	//
	int m_gamelogoCnt;

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 m_pDevice;
	// �쐬�e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTexSplitTitle[MAX_NUM_TEXTURE];
	// �쐬�e�N�X�`���T�[�t�F�C�X
	LPDIRECT3DSURFACE9	m_pSurfaceSplitTitle;
	// ��ʂ̃o�b�t�@�[
	LPDIRECT3DSURFACE9 m_pBackBuffer;

	//�^�C�g���Ɏg���e�N�X�`���̐�
	CSprite *m_spriteTitle[MAX_NUM_TEXTURE];

	//�Q�[�����S�p
	CSprite*m_pSpriteGamaeLogo[5];

	CFade *m_fade;
	//�e�N�X�`���[�̃��[�h
	void LoadTexture();
	void StartTitle();

};

#endif // !_TITLE_H_
