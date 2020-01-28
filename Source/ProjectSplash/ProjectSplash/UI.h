#pragma once

//---------インクルード---------
#include "main.h"
#include "sprite.h"


//--------クラス-----------
class CUI
{
public:
	CUI();
	virtual ~CUI() {};

	virtual void Init() = 0;
	virtual void Uninit()=0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void CreateUI(int camNum);
	static void CreateResultUI();
	static void AllInit();
	static void AllUninit();
	static void AllUpdate();
	static void AllDraw();

	int m_handle;
private:

protected:
	static std::vector<std::vector<CUI*>> CUI::m_list;
	static std::vector<CUI*> CUI::m_UI;
	int m_playerNo;
};
