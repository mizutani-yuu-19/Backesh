#pragma once

//---------�C���N���[�h---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//�񋓑�
enum UI_DAMAGE_TEXTURE	//UI�Ɏg���摜�̃i���o�����O
{
	NUM_DAMAGE = 0,	//���c�ʂ̔w�i

	MAX_DAMAGE_UI_TEXTURE,//�e�N�X�`���̍ő吔
};
//--------�N���X-----------
class CUIDamage :public CUI
{
public:

	// �R���X�g���N�^
	// �����P�F�v���C���[�l��
	// �����Q�F�v���C���[�i���o�[
	// UI�̔z�u�����߂�
	CUIDamage(int num, int No);

	~CUIDamage();

	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// �쐬�e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTex[MAX_DAMAGE_UI_TEXTURE];
	CSprite*			m_sprite[MAX_DAMAGE_UI_TEXTURE];
};
