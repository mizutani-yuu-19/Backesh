#include "UIresult.h"
#include "character.h"
#include "game.h"

//-------------�萔------------
#define UI_RESULT_PATH		("data/TEXTURE/UI/winner.png")

#define UI_RESULT_POS_X		(SCREEN_CENTER_X)
#define UI_RESULT_POS_Y		(SCREEN_CENTER_Y)
#define UI_RESULT_WIDTH		(SCREEN_WIDTH)
#define UI_RESULT_HEIGHT	(SCREEN_HEIGHT)

// �����P�F�v���C���[�l��
// �����Q�F�v���C���[�i���o�[
// UI�̔z�u�����߂�
CUIResult::CUIResult(int num, int No)
{
	m_playerNo = No;

	// �z�u
	D3DXVECTOR2 pos;
	float width;
	float height;
	for (int i = 0; i < MAX_RESULT_UI_TEXTURE; i++) {
		m_sprite[i] = new CSprite();
	}
	m_pTex[0] = SetTexture(UI_RESULT_PATH);

	pos.x = SCREEN_CENTER_X;
	pos.y = SCREEN_CENTER_Y;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	m_sprite[0]->InitSprite(pos, width, height, 1, 1);
}

CUIResult::~CUIResult()
{
	for (int i = 0; i < MAX_RESULT_UI_TEXTURE; i++) {
		SAFE_RELEASE(m_pTex[i]);
	}
	for (int i = 0; i < MAX_RESULT_UI_TEXTURE; i++) {
		SAFE_DELETE(m_sprite[i]);
	}
}

void CUIResult::Init() {

}
void CUIResult::Uninit() {

}
void CUIResult::Update() {
	switch (CCharacter::GetWinner()) {
	case 0:
		m_sprite[0]->m_color=PLAYER_COLOR1;
		break;
	case 1:
		m_sprite[0]->m_color = PLAYER_COLOR2;
		break;
	case 2:
		m_sprite[0]->m_color = PLAYER_COLOR3;
		break;
	case 3:
		m_sprite[0]->m_color = PLAYER_COLOR4;
		break;
	}
}
void CUIResult::Draw() {
	// �`�揇�͗񋓑��̏�
	for (int i = 0; i < MAX_RESULT_UI_TEXTURE; i++) {
		m_sprite[i]->DrawSprite(m_pTex[i]);
	}
}