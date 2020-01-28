#include "UItimer.h"
#include "game.h"

//-------------定数------------
#define UI_TIMER_PATH		("data/TEXTURE/UI/number.png")
#define UI_COLON_PATH		("data/TEXTURE/UI/colon.png")

#define UI_TIMER_POS_X		(SCREEN_CENTER_X)
#define UI_TIMER_POS_Y		(SCREEN_HEIGHT*0.04f)
#define UI_TIMER_WIDTH		(SCREEN_WIDTH*0.03f)
#define UI_TIMER_HEIGHT		(SCREEN_HEIGHT*0.03f)

CUITimer::CUITimer(int num, int No)
{
	m_playerNo = No;

	for (int i = 0; i < MAX_TIMER_UI_TEXTURE; i++) {
		m_sprite[i] = new CSprite();
	}
	// 一人
	if (num == 1) {
		pos.x = UI_TIMER_POS_X;
		pos.y = UI_TIMER_POS_Y;
		width = UI_TIMER_WIDTH;
		height = UI_TIMER_HEIGHT;
	}
	// 二人
	else if (num == 2) {
		pos.x = UI_TIMER_POS_X;
		pos.y = SCREEN_CENTER_Y * No + UI_TIMER_POS_Y;
		width = UI_TIMER_WIDTH;
		height = UI_TIMER_HEIGHT;
	}
	// 三人
	else if (num == 3 || num == 4) {
		pos.x = SCREEN_CENTER_X * (No % 2) + UI_TIMER_POS_X *0.5f;
		pos.y = SCREEN_CENTER_Y * (No / 2) + UI_TIMER_POS_Y*0.5f;
		width = UI_TIMER_WIDTH*0.5f;
		height = UI_TIMER_HEIGHT*0.5f;
	}
	m_pTex[NUM_TIMER] = SetTexture(UI_TIMER_PATH);
	m_pTex[NUM_COLON] = SetTexture(UI_COLON_PATH);
	m_sprite[NUM_TIMER]->InitSprite(pos, width, height, 10, 1);
	m_sprite[NUM_COLON]->InitSprite(pos, width, height, 1, 1);
}

CUITimer::~CUITimer()
{
	for (int i = 0; i < MAX_TIMER_UI_TEXTURE; i++) {
		SAFE_DELETE(m_sprite[i]);
		SAFE_RELEASE(m_pTex[i]);
	}
}

void CUITimer::Init() {

}
void CUITimer::Uninit() {

}
void CUITimer::Update() {

}
void CUITimer::Draw() {
	// 描画順は列挙隊の順
	int time = CGame::GetTime();
	int number[3];
	number[0] = time / 60;
	number[1] = (time % 60) / 10;
	number[2] = (time % 60) % 10;
	for (int i = 0; i < MAX_TIMER_UI_TEXTURE; i++) {
		if (i == NUM_TIMER) {
			for (int j = 0; j < 3; j++) {
				if (j == 0) {
					m_sprite[i]->m_pos = D3DXVECTOR2(pos.x - width, pos.y);
				}
				else {
					m_sprite[i]->m_pos = D3DXVECTOR2(pos.x + j * width, pos.y);
				}
				m_sprite[i]->m_patNum = number[j];
				m_sprite[i]->DrawSprite(m_pTex[i]);
			}
		}
		else {
			m_sprite[i]->DrawSprite(m_pTex[i]);
		}
	}
}