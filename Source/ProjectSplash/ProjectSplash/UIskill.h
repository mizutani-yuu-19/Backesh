#pragma once

//---------�C���N���[�h---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//�񋓑�
enum UI_SKILL_TEXTURE	//UI�Ɏg���摜�̃i���o�����O
{
	NUM_SKILL_BOTTLE = 0,	//���c�ʂ̔w�i
	NUM_SKILL_COLOR,		//����
	NUM_SKILL_STAR,		//�K�E�Z

	MAX_SKILL_UI_TEXTURE,//�e�N�X�`���̍ő吔
};
//--------�N���X-----------
class CUISkill:public CUI
{
public:

	// �R���X�g���N�^
	// �����P�F�v���C���[�l��
	// �����Q�F�v���C���[�i���o�[
	// UI�̔z�u�����߂�
	CUISkill(int num, int No);
	~CUISkill();
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// �쐬�e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTex[MAX_SKILL_UI_TEXTURE];
	CSprite*			m_sprite[MAX_SKILL_UI_TEXTURE];

	D3DXVECTOR2 pos;
	float width;
	float height;
};
