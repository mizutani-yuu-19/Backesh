//=======================================================
//			フェード イン/アウト処理 (fade.h)
//=======================================================
#ifndef _FADE_H_
#define _FADE_H_
//-------インクルード部-------
#include "main.h"
#include "scene_mgr.h"
//-------定数定義-------
#define VERTEX_NUM	(4)
#define FADE_TIME	(30)

//-------プロトタイプ宣言-------
class CFade
{
	public:
		CFade();
		~CFade();

		int UpdateFadeTex();

		void StartFadeInTex();
		void StartFadeOutTex();

		int EndFadeInTex();
		int EndFadeOutTex();

	protected:
		int m_fadeTexCount;
		int m_fadeTexDir;
		int m_alpha;


};

void InitFade();

void UpdateFade();
void DrawFade();

void StartFadeIn();
void StartFadeOut();

int EndFadeIn();
int EndFadeOut();

#endif
