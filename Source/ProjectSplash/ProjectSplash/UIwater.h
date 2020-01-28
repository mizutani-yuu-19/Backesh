#pragma once

//---------�C���N���[�h---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//�񋓑�
enum UI_WATER_TEXTURE	//UI�Ɏg���摜�̃i���o�����O
{
	NUM_WATER_BOTTLE = 0,	//���c�ʂ̔w�i
	NUM_WATER_COLOR,		//����
//	NUM_WATER_SKILL,		//�K�E�Z

	MAX_WATER_UI_TEXTURE,//�e�N�X�`���̍ő吔
};
//--------�N���X-----------
class CUIWater:public CUI
{
public:

	// �R���X�g���N�^
	// �����P�F�v���C���[�l��
	// �����Q�F�v���C���[�i���o�[
	// UI�̔z�u�����߂�
	CUIWater(int num, int No);
	~CUIWater();
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// �쐬�e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTex[MAX_WATER_UI_TEXTURE];
	CSprite*			m_sprite[MAX_WATER_UI_TEXTURE];

	D3DXVECTOR2 pos;
	float width;
	float height;
};
