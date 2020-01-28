#include "UIskill.h"
#include "character.h"

//-------------定数------------
#define UI_SKILL_POS_X		(SCREEN_WIDTH*0.2f)
#define UI_SKILL_POS_Y		(SCREEN_HEIGHT*0.2f)
#define UI_SKILL_WIDTH		(SCREEN_WIDTH*0.2f)
#define UI_SKILL_HEIGHT		(SCREEN_HEIGHT*0.03f)

#define UI_SKILL_COLOR_POS_X		(pos.x - width*0.47f)
#define UI_SKILL_COLOR_POS_Y		(pos.y)
#define UI_SKILL_COLOR_WIDTH		(width*0.95f)
#define UI_SKILL_COLOR_HEIGHT		(height*0.87f)

#define UI_SKILL_STAR_POS_X		(pos.x+SCREEN_WIDTH*0.1f/temp)
#define UI_SKILL_STAR_POS_Y		(pos.y)
#define UI_SKILL_STAR_WIDTH		(SCREEN_WIDTH*0.05f/temp)
#define UI_SKILL_STAR_HEIGHT	(SCREEN_WIDTH*0.05f/temp)

CUISkill::CUISkill(int num, int No)
{
	static char* path[MAX_SKILL_UI_TEXTURE] = {
		"data/TEXTURE/UI/water_bottle.png",
		"data/TEXTURE/UI/water_color.png",
		"data/TEXTURE/UI/skill.png", };

	m_playerNo = No;

	for (int i = 0; i < MAX_SKILL_UI_TEXTURE; i++) {
		m_sprite[i] = new CSprite();
	}
	// 一人
	if (num == 1) {
		width = UI_SKILL_WIDTH;
		height = UI_SKILL_HEIGHT;
		pos.x = UI_SKILL_POS_X;
		pos.y = UI_SKILL_POS_Y;
	}
	// 二人
	else if (num == 2) {
		width = UI_SKILL_WIDTH;
		height = UI_SKILL_HEIGHT;
		pos.x = UI_SKILL_POS_X;
		pos.y = SCREEN_CENTER_Y * No + UI_SKILL_POS_Y;
	}
	// 三人
	else if (num == 3 || num == 4) {
		width = UI_SKILL_WIDTH*0.5f;
		height = UI_SKILL_HEIGHT*0.5f;
		pos.x = SCREEN_CENTER_X * (No % 2) + UI_SKILL_POS_X * 0.5f;
		pos.y = SCREEN_CENTER_Y * (No / 2) + UI_SKILL_POS_Y * 0.5f;
	}
	for (int i = 0; i < MAX_SKILL_UI_TEXTURE; i++) {
		m_pTex[i] = SetTexture(path[i]);
	}
	int temp = (CGame::GetGameParam().playerNum-1) / 2 + 1;
	m_sprite[NUM_SKILL_BOTTLE]->InitSprite(pos, width, height, 1, 1);
	m_sprite[NUM_SKILL_COLOR]->InitSprite(D3DXVECTOR2(UI_SKILL_COLOR_POS_X, UI_SKILL_COLOR_POS_Y), UI_SKILL_COLOR_WIDTH, UI_SKILL_COLOR_HEIGHT, 1, 1);
	m_sprite[NUM_SKILL_COLOR]->m_color = D3DCOLOR_RGBA(255, 165, 0, 255);
	m_sprite[NUM_SKILL_STAR]->InitSprite(D3DXVECTOR2(UI_SKILL_STAR_POS_X, UI_SKILL_STAR_POS_Y), UI_SKILL_STAR_WIDTH, UI_SKILL_STAR_HEIGHT, 2, 1);
}

CUISkill::~CUISkill()
{
	for (int i = 0; i < MAX_SKILL_UI_TEXTURE; i++) {
		SAFE_DELETE(m_sprite[i]);
		SAFE_RELEASE(m_pTex[i]);
	}
}

void CUISkill::Init() {

}
void CUISkill::Uninit() {

}
void CUISkill::Update() {
	m_sprite[NUM_SKILL_COLOR]->m_width = CCharacter::GetPlayerSkillGauge(m_playerNo)*UI_SKILL_COLOR_WIDTH;
	m_sprite[NUM_SKILL_COLOR]->m_pos.x = UI_SKILL_COLOR_POS_X + CCharacter::GetPlayerSkillGauge(m_playerNo)*UI_SKILL_COLOR_WIDTH * 0.5f;

	if (CCharacter::GetPlayerSkillGauge(m_playerNo) >= 1.0f) {
		m_sprite[NUM_SKILL_STAR]->m_patNum = 0;
	}
	else {
		m_sprite[NUM_SKILL_STAR]->m_patNum = 1;
	}
}
void CUISkill::Draw() {
	// 描画順は列挙隊の順
	for (int i = 0; i < MAX_SKILL_UI_TEXTURE; i++) {
		m_sprite[i]->DrawSprite(m_pTex[i]);
	}
}