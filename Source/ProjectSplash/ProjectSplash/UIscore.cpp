#include "UIscore.h"
#include "character.h"

//-------------定数------------
// ボーダーの中心
#define UI_SCORE_BORDER_POS_X				(SCREEN_WIDTH*0.9f)
#define UI_SCORE_BORDER_POS_Y				(SCREEN_HEIGHT*0.1f)
#define UI_SCORE_BORDER_WIDTH				(SCREEN_WIDTH*0.075f)
#define UI_SCORE_BORDER_HEIGHT				(SCREEN_WIDTH*0.05f)

#define UI_SCORE_NUM_WIDTH					(width*0.15f)
#define UI_SCORE_NUM_HEIGHT					(height*0.30f)
CUIScore::CUIScore(int num, int No)
{
	static char* path[MAX_SCORE_UI_TEXTURE] = {
		"data/TEXTURE/UI/score_border.png",
		"data/TEXTURE/UI/score_num.png",
	};

	m_playerNo = No;

	for (int i = 0; i < MAX_SCORE_UI_TEXTURE; i++) {
		m_pTex[i] = SetTexture(path[i]);
	}

	for (int i = 0; i < MAX_SCORE_UI_TEXTURE; i++) {
		m_sprite[i] = new CSprite();
	}

	// ボーダーの位置
	// 一人
	if (num == 1) {
		width = UI_SCORE_BORDER_WIDTH * 2;
		height = UI_SCORE_BORDER_HEIGHT * 2;
		pos.x = UI_SCORE_BORDER_POS_X;
		pos.y = UI_SCORE_BORDER_POS_Y;
	}
	// 二人
	else if (num == 2) {
		width = UI_SCORE_BORDER_WIDTH * 1.5;
		height = UI_SCORE_BORDER_HEIGHT * 1.5f;
		pos.x = UI_SCORE_BORDER_POS_X+SCREEN_WIDTH*0.03f;
		pos.y = SCREEN_CENTER_Y * No + UI_SCORE_BORDER_POS_Y;
	}
	// 三人
	else if (num == 3 || num == 4) {
		width = UI_SCORE_BORDER_WIDTH;
		height = UI_SCORE_BORDER_HEIGHT;
		pos.x = SCREEN_CENTER_X * (No % 2) + UI_SCORE_BORDER_POS_X * 0.5f;
		pos.y = SCREEN_CENTER_Y * (No / 2) + UI_SCORE_BORDER_POS_Y * 0.5f;
	}
	m_sprite[NUM_SCORE_BORDER]->InitSprite(pos, width, height, 2, 1);

	m_sprite[NUM_SCORE_NUM]->InitSprite(D3DXVECTOR2(pos.x - UI_SCORE_BORDER_WIDTH*0.25f, pos.y - UI_SCORE_BORDER_HEIGHT*0.25f), UI_SCORE_NUM_WIDTH, UI_SCORE_NUM_HEIGHT, 10, 1);
	// ボーダーの種類
	if (CGame::GetGameParam().teamNum <= 2) {
		m_sprite[NUM_SCORE_BORDER]->m_patNum = 0;
	}
	else {
		m_sprite[NUM_SCORE_BORDER]->m_patNum = 1;
	}
}

CUIScore::~CUIScore()
{
	for (int i = 0; i < MAX_SCORE_UI_TEXTURE; i++) {
		SAFE_RELEASE(m_pTex[i]);
		SAFE_DELETE(m_sprite[i]);
	}
}

void CUIScore::Init() {

}
void CUIScore::Uninit() {

}
void CUIScore::Update() {

}
void CUIScore::Draw() {
	// 描画順は列挙隊の順
	for (int i = 0; i < MAX_SCORE_UI_TEXTURE; i++) {
		m_sprite[i]->DrawSprite(m_pTex[i]);
	}
	for (int i = 0; i < CGame::GetGameParam().teamNum; i++) {
		int number[3];
		int kill = CCharacter::GetKillCount(i);
		number[0] = kill / 100;
		number[1] = (kill % 100) / 10;
		number[2] = kill % 10;
		for (int j = 0; j < 3; j++) {
			if (j == 0) {
				switch (i)
				{
				case 0:
					m_sprite[NUM_SCORE_NUM]->m_color = PLAYER_COLOR1;
					break;
				case 1:
					m_sprite[NUM_SCORE_NUM]->m_color = PLAYER_COLOR2;
					break;
				case 2:
					m_sprite[NUM_SCORE_NUM]->m_color = PLAYER_COLOR3;
					break;
				case 3:
					m_sprite[NUM_SCORE_NUM]->m_color = PLAYER_COLOR4;
					break;
				default:
					break;
				}
				m_sprite[NUM_SCORE_NUM]->m_pos = D3DXVECTOR2(pos.x - width*0.4f + width*0.5f*(i % 2), pos.y - height*0.25f + height*0.5f*(i / 2));
			}
			else {
				m_sprite[NUM_SCORE_NUM]->m_pos += D3DXVECTOR2(UI_SCORE_NUM_WIDTH, 0);
			}
			m_sprite[NUM_SCORE_NUM]->m_patNum = number[j];
			m_sprite[NUM_SCORE_NUM]->DrawSprite(m_pTex[NUM_SCORE_NUM]);
		}
	}
}