#include "UIdamage.h"
#include "character.h"

//-------------�萔------------
#define UI_DAMAGE_PATH		("data/TEXTURE/UI/damage.png")

#define UI_DAMAGE_POS_X		(SCREEN_CENTER_X)
#define UI_DAMAGE_POS_Y		(SCREEN_CENTER_Y)
#define UI_DAMAGE_WIDTH		(SCREEN_WIDTH)
#define UI_DAMAGE_HEIGHT	(SCREEN_HEIGHT)

// �����P�F�v���C���[�l��
// �����Q�F�v���C���[�i���o�[
// UI�̔z�u�����߂�
CUIDamage::CUIDamage(int num, int No)
{
	m_playerNo = No;

	// �z�u
	D3DXVECTOR2 pos;
	float width;
	float height;
	for (int i = 0; i < MAX_DAMAGE_UI_TEXTURE; i++) {
		m_sprite[i] = new CSprite();
	}
	m_pTex[NUM_DAMAGE] = SetTexture(UI_DAMAGE_PATH);

	// ��l
	if (num == 1) {
		pos.x = SCREEN_CENTER_X;
		pos.y = SCREEN_CENTER_Y;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
	}
	// ��l
	else if (num == 2) {
		pos.x = SCREEN_CENTER_X;
		pos.y = SCREEN_CENTER_Y*No + SCREEN_CENTER_Y / 2;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT / 2;
	}
	// �O�l
	else if (num == 3||num == 4) {
		pos.x = SCREEN_CENTER_X * (No % 2) + SCREEN_CENTER_X / 2;
		pos.y = SCREEN_CENTER_Y * (No / 2) + SCREEN_CENTER_Y / 2;
		width = SCREEN_WIDTH / 2;
		height = SCREEN_HEIGHT / 2;
	}

	m_sprite[NUM_DAMAGE]->InitSprite(pos, width, height, 1, 1);
}

CUIDamage::~CUIDamage()
{
	for (int i = 0; i < MAX_DAMAGE_UI_TEXTURE; i++) {
		SAFE_DELETE(m_sprite[i]);
		SAFE_RELEASE(m_pTex[i]);
	}
}

void CUIDamage::Init() {

}
void CUIDamage::Uninit() {

}
void CUIDamage::Update() {
	if (CCharacter::GetPlayerLife(m_playerNo) < 0) {
		m_sprite[NUM_DAMAGE]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		return;
	}
	m_sprite[NUM_DAMAGE]->m_color = D3DCOLOR_ARGB((int)(255 - (CCharacter::GetPlayerLife(m_playerNo)*255.0f)), 255, 255, 255);
}
void CUIDamage::Draw() {
	// �`�揇�͗񋓑��̏�
	for (int i = 0; i < MAX_DAMAGE_UI_TEXTURE; i++) {
		m_sprite[i]->DrawSprite(m_pTex[i]);
	}
}