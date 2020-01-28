//=======================================================
//			�t�F�[�h �C��/�A�E�g���� (fade.h)
//=======================================================
#ifndef _FADE_H_
#define _FADE_H_
//-------�C���N���[�h��-------
#include "main.h"
#include "scene_mgr.h"
//-------�萔��`-------
#define VERTEX_NUM	(4)
#define FADE_TIME	(30)

//-------�v���g�^�C�v�錾-------
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
