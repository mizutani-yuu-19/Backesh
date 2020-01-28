#include "UI.h"
#include "UIwater.h"
#include "UIdamage.h"
#include "UItimer.h"
#include "UIstartcount.h"
#include "UIplayer.h"
#include "UIscore.h"
#include "UIskill.h"
#include "UIresult.h"

//std::vector<CUI*>	CUI::m_UI;
std::vector<std::vector<CUI*>>	CUI::m_list;

void CUI::CreateUI(int camNum) {
	std::vector<CUI*> tempList;
	CUI*	temp;
	for (int i = 0; i < camNum; i++) {
		m_list.push_back(tempList);
		temp = new CUIDamage(camNum, i);
		m_list[i].push_back(temp);
		temp = new CUIWater(camNum, i);
		m_list[i].push_back(temp);
		temp = new CUISkill(camNum, i);
		m_list[i].push_back(temp);
		temp = new CUITimer(camNum, i);
		m_list[i].push_back(temp);
		temp = new CUIPlayer(camNum, i);
		m_list[i].push_back(temp);
		temp = new CUIScore(camNum, i);
		m_list[i].push_back(temp);
	}
	temp = new CUIStartCount(camNum, 0);
	temp->m_handle = 4;
	m_list.push_back(tempList);
	m_list[camNum].push_back(temp);
}

void CUI::CreateResultUI() {
	std::vector<CUI*> tempList;
	CUI*	temp;
	m_list.push_back(tempList);
	temp = new CUIScore(1, 0);
	m_list[0].push_back(temp);
	temp = new CUIResult(1, 0);
	m_list[0].push_back(temp);
}

void CUI::AllInit() {
	for (int j = 0; j < m_list.size(); j++) {
		for (int i = 0; i < m_list[j].size(); i++) {
			m_list[j][i]->Init();
		}
	}
}
void CUI::AllUninit() {
	for (int j = 0; j < m_list.size(); j++) {
		for (int i = 0; i < m_list[j].size(); i++) {
			m_list[j][i]->Uninit();
			delete m_list[j][i];
		}
	}
	m_list.clear();
}
void CUI::AllUpdate() {
	for (int j = 0; j < m_list.size(); j++) {
		for (int i = 0; i < m_list[j].size(); i++) {
			m_list[j][i]->Update();
		}
	}
}

void CUI::AllDraw() {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DVIEWPORT9 port = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0.0f,0.0f };
	pDevice->SetViewport(&port);
	for (int j = 0; j < m_list.size(); j++) {
		for (int i = 0; i < m_list[j].size(); i++) {
			m_list[j][i]->Draw();
		}
	}
	//SAFE_RELEASE(pDevice);
	pDevice = NULL;
}

CUI::CUI()
{
}
