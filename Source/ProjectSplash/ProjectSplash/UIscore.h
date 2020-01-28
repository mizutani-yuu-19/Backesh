#pragma once

//---------�C���N���[�h---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//�񋓑�
enum UI_SCORE_TEXTURE	//UI�Ɏg���摜�̃i���o�����O
{
	NUM_SCORE_BORDER = 0,	// ��
	NUM_SCORE_NUM,			// ����

	MAX_SCORE_UI_TEXTURE,	//�e�N�X�`���̍ő吔
};
//--------�N���X-----------
class CUIScore:public CUI
{
public:

	// �R���X�g���N�^
	// �����P�F�v���C���[�l��
	// �����Q�F�v���C���[�i���o�[
	// UI�̔z�u�����߂�
	CUIScore(int num, int No);
	~CUIScore();
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// �쐬�e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTex[MAX_SCORE_UI_TEXTURE];
	CSprite*			m_sprite[MAX_SCORE_UI_TEXTURE];

	D3DXVECTOR2 pos;
	float width;
	float height;
};
