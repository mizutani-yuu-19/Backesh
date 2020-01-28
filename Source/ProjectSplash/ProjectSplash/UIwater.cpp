#include "UIwater.h"
#include "character.h"

//-------------定数------------

#define UI_WATER_POS_X		(SCREEN_WIDTH*0.2f)
#define UI_WATER_POS_Y		(SCREEN_HEIGHT*0.15f)
#define UI_WATER_WIDTH		(SCREEN_WIDTH*0.2f)
#define UI_WATER_HEIGHT		(SCREEN_HEIGHT*0.05f)

#define UI_WATER_COLOR_POS_X		(pos.x - width*0.47f)
#define UI_WATER_COLOR_POS_Y		(pos.y)
#define UI_WATER_COLOR_WIDTH		(width*0.95f)
#define UI_WATER_COLOR_HEIGHT		(height*0.85f)

//#define UI_SKILL_POS_X		(pos.x+SCREEN_WIDTH*0.1f/temp)
//#define UI_SKILL_POS_Y		(pos.y)
//#define UI_SKILL_WIDTH		(SCREEN_WIDTH*0.05f/temp)
//#define UI_SKILL_HEIGHT		(SCREEN_WIDTH*0.05f/temp)

CUIWater::CUIWater(int num, int No)
{
	static char* path[MAX_WATER_UI_TEXTURE] = {
		"data/TEXTURE/UI/water_bottle.png",
		"data/TEXTURE/UI/water_color.png",
//		"data/TEXTURE/UI/skill.png",
	};

	m_playerNo = No;

	for (int i = 0; i < MAX_WATER_UI_TEXTURE; i++) {
		m_sprite[i] = new CSprite();
	}
	// 一人
	if (num == 1) {
		width = UI_WATER_WIDTH;
		height = UI_WATER_HEIGHT;
		pos.x = UI_WATER_POS_X;
		pos.y = UI_WATER_POS_Y;
	}
	// 二人
	else if (num == 2) {
		width = UI_WATER_WIDTH;
		height = UI_WATER_HEIGHT;
		pos.x = UI_WATER_POS_X;
		pos.y = SCREEN_CENTER_Y * No + UI_WATER_POS_Y;
	}
	// 三人
	else if (num == 3 || num == 4) {
		width = UI_WATER_WIDTH*0.5f;
		height = UI_WATER_HEIGHT*0.5f;
		pos.x = SCREEN_CENTER_X * (No % 2) + UI_WATER_POS_X * 0.5f;
		pos.y = SCREEN_CENTER_Y * (No / 2) + UI_WATER_POS_Y * 0.5f;
	}
	for (int i = 0; i < MAX_WATER_UI_TEXTURE; i++) {
		m_pTex[i] = SetTexture(path[i]);
	}
	int temp = (CGame::GetGameParam().playerNum-1) / 2 + 1;
	m_sprite[NUM_WATER_BOTTLE]->InitSprite(pos, width, height, 1, 1);
	m_sprite[NUM_WATER_COLOR]->InitSprite(D3DXVECTOR2(UI_WATER_COLOR_POS_X, UI_WATER_COLOR_POS_Y), UI_WATER_COLOR_WIDTH, UI_WATER_COLOR_HEIGHT, 1, 1);
	m_sprite[NUM_WATER_COLOR]->m_color = D3DCOLOR_RGBA(0, 0, 255, 255);
//	m_sprite[NUM_WATER_SKILL]->InitSprite(D3DXVECTOR2(UI_SKILL_POS_X, UI_SKILL_POS_Y), UI_SKILL_WIDTH, UI_SKILL_HEIGHT, 2, 1);
}

CUIWater::~CUIWater()
{
	for (int i = 0; i < MAX_WATER_UI_TEXTURE; i++) {
		SAFE_DELETE(m_sprite[i]);
		SAFE_RELEASE(m_pTex[i]);
	}
}

void CUIWater::Init() {

}
void CUIWater::Uninit() {

}
void CUIWater::Update() {
	m_sprite[NUM_WATER_COLOR]->m_width = CCharacter::GetPlayerWaterVol(m_playerNo)*UI_WATER_COLOR_WIDTH;
	m_sprite[NUM_WATER_COLOR]->m_pos.x = UI_WATER_COLOR_POS_X + CCharacter::GetPlayerWaterVol(m_playerNo)*UI_WATER_COLOR_WIDTH * 0.5f;

	//if (CCharacter::GetPlayerWaterVol(m_playerNo) >= 1.0f) {
	//	m_sprite[NUM_WATER_SKILL]->m_patNum = 0;
	//}
	//else {
	//	m_sprite[NUM_WATER_SKILL]->m_patNum = 1;
	//}
}
void CUIWater::Draw() {
	// 描画順は列挙隊の順
	for (int i = 0; i < MAX_WATER_UI_TEXTURE; i++) {
		m_sprite[i]->DrawSprite(m_pTex[i]);
	}
}