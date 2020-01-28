#pragma once

//---------�C���N���[�h---------
#include "main.h"
#include "sprite.h"
#include "UI.h"
#include "RGBConvert.h"

//�񋓑�
enum UI_PLAYER_TEXTURE	//UI�Ɏg���摜�̃i���o�����O
{
	NUM_PLAYER_BACK,		// �L�����N�^�[�w�i
	NUM_PLAYER_LOUCUST,		// �o�b�^
	NUM_PLAYER_FIREFLY,		// �u
	NUM_PLAYER_BETTLE,		// �J�u�g���V
	NUM_PLAYER_CIRCLE,		// ����̉~

	MAX_PLAYER_UI_TEXTURE,//�e�N�X�`���̍ő吔
};

#define UI_PLAYER_DRAW_NUM (9)
//--------�N���X-----------
class CUIPlayer:public CUI
{
public:
	// �R���X�g���N�^
	// �����P�F�v���C���[�l��
	// �����Q�F�v���C���[�i���o�[
	// UI�̔z�u�����߂�
	CUIPlayer(int num, int No);
	~CUIPlayer();
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// �쐬�e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTex[MAX_PLAYER_UI_TEXTURE];
	CSprite*			m_sprite[UI_PLAYER_DRAW_NUM];

	int m_playerType[3];

	D3DXVECTOR2 pos;
	float width;
	float height;

	HSVA hsva[3];
};
