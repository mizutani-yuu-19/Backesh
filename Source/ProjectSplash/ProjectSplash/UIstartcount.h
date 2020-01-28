#pragma once

//---------�C���N���[�h---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//�񋓑�
enum UI_STARTCOUNT_TEXTURE	//UI�Ɏg���摜�̃i���o�����O
{
	NUM_STARTCOUNT_1_UI_TEXTURE,
	NUM_STARTCOUNT_2_UI_TEXTURE,
	NUM_STARTCOUNT_3_UI_TEXTURE,
	NUM_STARTCOUNT_S_UI_TEXTURE,
	NUM_STARTCOUNT_T_UI_TEXTURE,
	NUM_STARTCOUNT_A_UI_TEXTURE,
	NUM_STARTCOUNT_R_UI_TEXTURE,
	NUM_STARTCOUNT_T2_UI_TEXTURE,
	MAX_STARTCOUNT_UI_TEXTURE
};
//--------�N���X-----------
class CUIStartCount :public CUI
{
public:

	// �R���X�g���N�^
	// �����P�F�v���C���[�l��
	// �����Q�F�v���C���[�i���o�[
	// UI�̔z�u�����߂�
	CUIStartCount(int num, int No);

	~CUIStartCount();

	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// �쐬�e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTex[MAX_STARTCOUNT_UI_TEXTURE];
	CSprite*			m_sprite[MAX_STARTCOUNT_UI_TEXTURE];

	int m_a[MAX_STARTCOUNT_UI_TEXTURE];
	int m_time;
	int m_step;
};
