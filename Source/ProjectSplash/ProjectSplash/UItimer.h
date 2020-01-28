#pragma once

//---------�C���N���[�h---------
#include "main.h"
#include "sprite.h"
#include "UI.h"

//�񋓑�
enum UI_TIMER_TEXTURE	//UI�Ɏg���摜�̃i���o�����O
{
	NUM_COLON = 0,		// �R����
	NUM_TIMER,			// ����

	MAX_TIMER_UI_TEXTURE,//�e�N�X�`���̍ő吔
};
//--------�N���X-----------
class CUITimer :public CUI
{
public:
	CUITimer(int num, int No);
	~CUITimer();

	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	// �쐬�e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTex[MAX_TIMER_UI_TEXTURE];
	CSprite*			m_sprite[MAX_TIMER_UI_TEXTURE];

	D3DXVECTOR2 pos;
	float width;
	float height;
};
