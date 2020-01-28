#include "UIplayer.h"
#include "character.h"
//-------------定数------------

#define UI_PLAYER_WIDTH		(SCREEN_WIDTH*0.05f)
#define UI_PLAYER_HEIGHT	(SCREEN_WIDTH*0.05f)
#define UI_PLAYER_POS_X		(width*0.5f+SCREEN_WIDTH*0.01f)
#define UI_PLAYER_POS_Y		(height*0.5f+SCREEN_WIDTH*0.01f)

#define UI_SUB_WIDTH		(width*0.6f)
#define UI_SUB_HEIGHT		(height*0.6f)

#define UI_SUB1_POS_X		(pos.x + width*0.5f+UI_SUB_WIDTH)
#define UI_SUB1_POS_Y		(pos.y - height*0.5f+UI_SUB_HEIGHT*0.5f)

#define UI_SUB2_POS_X		(UI_SUB1_POS_X + UI_SUB_WIDTH)
#define UI_SUB2_POS_Y		(UI_SUB1_POS_Y)

CUIPlayer::CUIPlayer(int num, int No)
{
	static char* path[MAX_PLAYER_UI_TEXTURE] = {
		"data/TEXTURE/UI/character_back.png",
		"data/TEXTURE/UI/loucust.png",
		"data/TEXTURE/UI/firefly.png",
		"data/TEXTURE/UI/beetle.png",
		"data/TEXTURE/UI/circle.png" };
	m_playerNo = No;

	for (int i = 0; i < UI_PLAYER_DRAW_NUM; i++) {
		m_sprite[i] = new CSprite();
	}
	// 一人
	if (num == 1) {
		width = UI_PLAYER_WIDTH * 2;
		height = UI_PLAYER_HEIGHT * 2;
		pos.x = UI_PLAYER_POS_X;
		pos.y = UI_PLAYER_POS_Y;
	}
	// 二人
	else if (num == 2) {
		width = UI_PLAYER_WIDTH * 2;
		height = UI_PLAYER_HEIGHT * 1.7;
		pos.x = UI_PLAYER_POS_X;
		pos.y = SCREEN_CENTER_Y * No + UI_PLAYER_POS_Y;
	}
	// 三人
	else if (num == 3 || num == 4) {
		width = UI_PLAYER_WIDTH;
		height = UI_PLAYER_HEIGHT;
		pos.x = SCREEN_CENTER_X * (No % 2) + UI_PLAYER_POS_X;
		pos.y = SCREEN_CENTER_Y * (No / 2) + UI_PLAYER_POS_Y;
	}
	// プレイヤー
	for (int i = 0; i < 3; i++) {
		m_sprite[i * 3]->InitSprite(pos, width, height, 1, 1);
		m_sprite[1 + i * 3]->InitSprite(D3DXVECTOR2(UI_SUB1_POS_X, UI_SUB1_POS_Y), UI_SUB_WIDTH, UI_SUB_HEIGHT, 1, 1);
		m_sprite[2 + i * 3]->InitSprite(D3DXVECTOR2(UI_SUB2_POS_X, UI_SUB2_POS_Y), UI_SUB_WIDTH, UI_SUB_HEIGHT, 1, 1);
	}
	for (int i = 0; i < 3; i++) {
		m_sprite[i + 3]->m_scale = D3DXVECTOR2(1.05f, 1.05f);
		m_sprite[6 + i]->m_scale = D3DXVECTOR2(0.8f,0.8f);
	}
	for (int i = 0; i < MAX_PLAYER_UI_TEXTURE; i++) {
		m_pTex[i] = SetTexture(path[i]);
	}
	for (int i = 0; i < 3; i++) {
		switch (No)
		{
		case 0:
			m_sprite[i]->m_color = PLAYER_COLOR1;
			break;
		case 1:
			m_sprite[i]->m_color = PLAYER_COLOR2;
			break;
		case 2:
			m_sprite[i]->m_color = PLAYER_COLOR3;
			break;
		case 3:
			m_sprite[i]->m_color = PLAYER_COLOR4;
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < 3; i++) {
		m_playerType[i] = -1;
		hsva[i].H = 0;
		hsva[i].S = 100;
		hsva[i].V = 50;
		hsva[i].A = 255;
	}

}

CUIPlayer::~CUIPlayer()
{
	for (int i = 0; i < MAX_PLAYER_UI_TEXTURE; i++) {
		SAFE_RELEASE(m_pTex[i]);
	}
	for (int i = 0; i < UI_PLAYER_DRAW_NUM; i++) {
		SAFE_DELETE(m_sprite[i]);
	}
}

void CUIPlayer::Init() {

}

void CUIPlayer::Uninit() {

}

void CUIPlayer::Update() {
	for (int i = 0; i < 3; i++) {
		int b= CCharacter::GetCharacterContentPos(m_playerNo, i);
		if (m_playerType[i] != CCharacter::GetCharacterType(m_playerNo, b)) {
			m_playerType[i] = CCharacter::GetCharacterType(m_playerNo, b);
		}
		float a = CCharacter::GetCharacterLife(m_playerNo, b);
		if (a < 0) {
			hsva[i].H = 0;
		}
		if (hsva[i].H != 100 - (1 - a) * 100) {
			hsva[i].H = 100 - (1 - a) * 100;
			m_sprite[3 + i]->m_color = HSVConvert(hsva[i]);
		}
	}
}

void CUIPlayer::Draw() {
	// わっか
	for (int i = 0; i < 3; i++) {
		m_sprite[3 + i]->DrawSprite(m_pTex[NUM_PLAYER_CIRCLE]);
	}
	// キャラ背景
	for (int i = 0; i < 3; i++) {
		m_sprite[i]->DrawSprite(m_pTex[NUM_PLAYER_BACK]);
	}
	// キャラ画
	for (int i = 0; i < 3; i++) {
		m_sprite[6 + i]->DrawSprite(m_pTex[1 + m_playerType[i]]);
	}
}