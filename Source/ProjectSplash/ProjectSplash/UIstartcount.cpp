#include "UIstartcount.h"
#include "character.h"

//-------------定数------------
#define UI_STARTCOUNT_WIDTH (SCREEN_WIDTH/5)
#define UI_STARTCOUNT_HEIGHT (SCREEN_WIDTH/5)
#define UI_START_WIDTH (SCREEN_WIDTH/10)
#define UI_START_HEIGHT (SCREEN_WIDTH/10)

// 引数１：プレイヤー人数
// 引数２：プレイヤーナンバー
// UIの配置を決める
CUIStartCount::CUIStartCount(int num, int No)
{
	static char* path[MAX_STARTCOUNT_UI_TEXTURE] = {
		"data/TEXTURE/UI/1.png",
		"data/TEXTURE/UI/2.png",
		"data/TEXTURE/UI/3.png",
		"data/TEXTURE/UI/s.png",
		"data/TEXTURE/UI/t.png",
		"data/TEXTURE/UI/a.png",
		"data/TEXTURE/UI/r.png",
		"data/TEXTURE/UI/t.png",
	};

	m_time = 0;

	// 配置
	D3DXVECTOR2 pos;
	float width;
	float height;

	pos.x = SCREEN_CENTER_X;
	pos.y = SCREEN_CENTER_Y;
	width = UI_STARTCOUNT_WIDTH;
	height = UI_STARTCOUNT_HEIGHT;

	for (int i = 0; i < MAX_STARTCOUNT_UI_TEXTURE; i++) {
		m_sprite[i] = new CSprite();
		m_pTex[i] = SetTexture(path[i]);
		m_sprite[i]->InitSprite(pos, width, height, 1, 1);
		m_a[i] = 0;
		m_sprite[i]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
	}
}

CUIStartCount::~CUIStartCount()
{
	for (int i = 0; i < MAX_STARTCOUNT_UI_TEXTURE; i++) {
		SAFE_RELEASE(m_pTex[i]);
		SAFE_DELETE(m_sprite[i]);
	}
}

void CUIStartCount::Init() {

}
void CUIStartCount::Uninit() {

}
void CUIStartCount::Update() {
	// ステップ切り替えと時間ごとの初期化
	if (CGame::GetStep() != STEP_COUNTDOWN && CGame::GetStep() != STEP_GAME)	return;
	switch (m_time)
	{
	case 0:
		m_step = 0;
		m_sprite[NUM_STARTCOUNT_3_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		break;
	case 60:
		m_step = 1;
		m_sprite[NUM_STARTCOUNT_3_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
		m_sprite[NUM_STARTCOUNT_2_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		break;
	case 120:
		m_step = 2;
		m_sprite[NUM_STARTCOUNT_2_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
		m_sprite[NUM_STARTCOUNT_1_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		break;
	case 180:
		m_step = 3;
		m_sprite[NUM_STARTCOUNT_1_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);

		m_sprite[NUM_STARTCOUNT_S_UI_TEXTURE]->InitSprite(D3DXVECTOR2(SCREEN_CENTER_X - UI_START_WIDTH * 2, SCREEN_CENTER_Y), UI_START_WIDTH, UI_START_HEIGHT, 1, 1);
		m_sprite[NUM_STARTCOUNT_T_UI_TEXTURE]->InitSprite(D3DXVECTOR2(SCREEN_CENTER_X - UI_START_WIDTH, SCREEN_CENTER_Y - 20), UI_START_WIDTH, UI_START_HEIGHT, 1, 1);
		m_sprite[NUM_STARTCOUNT_A_UI_TEXTURE]->InitSprite(D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y - 30), UI_START_WIDTH, UI_START_HEIGHT, 1, 1);
		m_sprite[NUM_STARTCOUNT_R_UI_TEXTURE]->InitSprite(D3DXVECTOR2(SCREEN_CENTER_X + UI_START_WIDTH, SCREEN_CENTER_Y - 20), UI_START_WIDTH, UI_START_HEIGHT, 1, 1);
		m_sprite[NUM_STARTCOUNT_T2_UI_TEXTURE]->InitSprite(D3DXVECTOR2(SCREEN_CENTER_X + UI_START_WIDTH * 2, SCREEN_CENTER_Y), UI_START_WIDTH, UI_START_HEIGHT, 1, 1);

		m_sprite[NUM_STARTCOUNT_S_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		m_sprite[NUM_STARTCOUNT_T_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		m_sprite[NUM_STARTCOUNT_A_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		m_sprite[NUM_STARTCOUNT_R_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		m_sprite[NUM_STARTCOUNT_T2_UI_TEXTURE]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
		CGame::SetStep(STEP_GAME);
		break;
	case 240:

		for (int j = 0; j < m_list.size(); j++) {
			for (int i = 0; i < m_list[j].size(); i++) {
				if (m_list[j][i]->m_handle == 4)
				{
					m_list[j].erase(m_list[j].begin() + i);
					delete this;
				}

			}
		}
		break;
	default:
		break;
	}
	// ステップごとのアップデート
	switch (m_step)
	{
	case 0:

		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		if (m_sprite[0]) {
			m_sprite[NUM_STARTCOUNT_S_UI_TEXTURE]->m_scale -= D3DXVECTOR2(0.01f, 0.01f);
			m_sprite[NUM_STARTCOUNT_T_UI_TEXTURE]->m_scale -= D3DXVECTOR2(0.01f, 0.01f);
			m_sprite[NUM_STARTCOUNT_A_UI_TEXTURE]->m_scale -= D3DXVECTOR2(0.01f, 0.01f);
			m_sprite[NUM_STARTCOUNT_R_UI_TEXTURE]->m_scale -= D3DXVECTOR2(0.01f, 0.01f);
			m_sprite[NUM_STARTCOUNT_T2_UI_TEXTURE]->m_scale -= D3DXVECTOR2(0.01f, 0.01f);

			m_sprite[NUM_STARTCOUNT_S_UI_TEXTURE]->m_pos += D3DXVECTOR2(-1.0f, -0.5f);
			m_sprite[NUM_STARTCOUNT_T_UI_TEXTURE]->m_pos += D3DXVECTOR2(-0.5f, -1.0f);
			m_sprite[NUM_STARTCOUNT_A_UI_TEXTURE]->m_pos += D3DXVECTOR2(0.0f, -1.5f);
			m_sprite[NUM_STARTCOUNT_R_UI_TEXTURE]->m_pos += D3DXVECTOR2(+0.5f, -1.0f);
			m_sprite[NUM_STARTCOUNT_T2_UI_TEXTURE]->m_pos += D3DXVECTOR2(+1.0f, -0.5f);

			m_sprite[NUM_STARTCOUNT_S_UI_TEXTURE]->m_angleDegree -= 0.5f;
			m_sprite[NUM_STARTCOUNT_T_UI_TEXTURE]->m_angleDegree -= 0.8f;
			m_sprite[NUM_STARTCOUNT_A_UI_TEXTURE]->m_angleDegree += 1.5f;
			m_sprite[NUM_STARTCOUNT_R_UI_TEXTURE]->m_angleDegree += 0.8f;
			m_sprite[NUM_STARTCOUNT_T2_UI_TEXTURE]->m_angleDegree += 0.5f;
		}
		break;
	default:
		break;
	}

	m_time++;
}
void CUIStartCount::Draw() {
	// 描画順は列挙隊の順
	for (int i = 0; i < MAX_STARTCOUNT_UI_TEXTURE; i++) {
		m_sprite[i]->DrawSprite(m_pTex[i]);
	}
}