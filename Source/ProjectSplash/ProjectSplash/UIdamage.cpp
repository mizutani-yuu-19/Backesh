#include "UIdamage.h"
#include "character.h"

//-------------定数------------
#define UI_DAMAGE_PATH		("data/TEXTURE/UI/damage.png")

#define UI_DAMAGE_POS_X		(SCREEN_CENTER_X)
#define UI_DAMAGE_POS_Y		(SCREEN_CENTER_Y)
#define UI_DAMAGE_WIDTH		(SCREEN_WIDTH)
#define UI_DAMAGE_HEIGHT	(SCREEN_HEIGHT)

// 引数１：プレイヤー人数
// 引数２：プレイヤーナンバー
// UIの配置を決める
CUIDamage::CUIDamage(int num, int No)
{
	m_playerNo = No;

	// 配置
	D3DXVECTOR2 pos;
	float width;
	float height;
	for (int i = 0; i < MAX_DAMAGE_UI_TEXTURE; i++) {
		m_sprite[i] = new CSprite();
	}
	m_pTex[NUM_DAMAGE] = SetTexture(UI_DAMAGE_PATH);

	// 一人
	if (num == 1) {
		pos.x = SCREEN_CENTER_X;
		pos.y = SCREEN_CENTER_Y;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
	}
	// 二人
	else if (num == 2) {
		pos.x = SCREEN_CENTER_X;
		pos.y = SCREEN_CENTER_Y*No + SCREEN_CENTER_Y / 2;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT / 2;
	}
	// 三人
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
	// 描画順は列挙隊の順
	for (int i = 0; i < MAX_DAMAGE_UI_TEXTURE; i++) {
		m_sprite[i]->DrawSprite(m_pTex[i]);
	}
}