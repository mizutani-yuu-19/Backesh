//=======================================================
//
// �t�F�[�h �C��/�A�E�g���� (fade.cpp)
//
//=======================================================
#include "fade.h"

//----- �}�N����`

//=======================================================
//�@�\�@���@�́@��@��
//=======================================================

//=======================================================
//�@�O�@���@�[�@�o�@���@�ρ@��
//=======================================================

VERTEX_2D	g_vertex[VERTEX_NUM];	//���_���

int	g_fadeCount;
int g_nDir;

int	g_nRed;			// �t�F�[�h�A�E�g�F(R)
int	g_nGreen;		// �t�F�[�h�A�E�g�F(G)
int	g_nBlue;		// �t�F�[�h�A�E�g�F(B)

static LPDIRECT3DDEVICE9 g_pDevice;


//=======================================================
//�@�R�@���@�X�@�g�@���@�N�@�^
//=======================================================
CFade::CFade()
{
	m_fadeTexDir = -1;
	m_fadeTexCount = 0;
	m_alpha = 0;

}

//=======================================================
//�@�f�@�X�@�g�@���@�N�@�^
//=======================================================
CFade::~CFade()
{

}

//=======================================================
//	�e�N�X�`���p�̍X�V����
//=======================================================
int CFade::UpdateFadeTex()
{
	m_fadeTexCount += m_fadeTexDir;

	if (m_fadeTexCount < 0)
	{
		m_fadeTexCount = 0;
	}

	else if (m_fadeTexCount > FADE_TIME)
	{
		m_fadeTexCount = FADE_TIME;
	}
	
	m_alpha = m_fadeTexCount * 255 / FADE_TIME;
	
	return m_alpha;
}

//=======================================================
//	 �e�N�X�`���t�F�[�h�C���J�n
//=======================================================
void CFade::StartFadeInTex()
{
	m_fadeTexCount = FADE_TIME;
	m_fadeTexDir = -1;
}
//=======================================================
//	 �e�N�X�`���t�F�[�h�A�E�g�J�n
//=======================================================
void CFade::StartFadeOutTex()
{
	m_fadeTexCount = 0;
	m_fadeTexDir = 1;
}

//=======================================================
//�@�e�N�X�`���t�F�[�h�C���I��
//=======================================================
int CFade::EndFadeInTex()
{
	return (m_fadeTexDir < 0 && m_fadeTexCount <= 0);
}

//=======================================================
//�@�e�N�X�`���t�F�[�h�A�E�g�I��
//=======================================================
int CFade::EndFadeOutTex()
{
	return (m_fadeTexDir > 0 && m_fadeTexCount >= FADE_TIME);
}

//=======================================================
//�@���@���@���@���@��
//=======================================================
void InitFade()
{
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		g_vertex[i].uv = D3DXVECTOR2(i & 1, i >> 1);
		g_vertex[i].vtxPos.x = g_vertex[i].uv.x * SCREEN_WIDTH;
		g_vertex[i].vtxPos.y = g_vertex[i].uv.y * SCREEN_HEIGHT;
		g_vertex[i].vtxPos.z = 0.0f;
		g_vertex[i].rhw = 1.0f;
		g_vertex[i].diffuse = D3DCOLOR_ARGB(0, 0, 0, 0);
	}
	g_fadeCount = -1;
	g_nDir = -1;
	g_nRed = 0;
	g_nBlue = 0;
	g_nGreen = 0;
}
//=======================================================
//�@�X�@�V�@���@��
//=======================================================
void UpdateFade()
{
	g_fadeCount += g_nDir;

	if (g_fadeCount < 0)
	{
		g_fadeCount = 0;
	}

	else if (g_fadeCount > FADE_TIME)
	{
		g_fadeCount = FADE_TIME;
	}

	int nAlpha = g_fadeCount * 255 / FADE_TIME;

	D3DCOLOR color = D3DCOLOR_ARGB(nAlpha, g_nRed, g_nGreen, g_nBlue);

	for (int i = 0; i < 4; ++i)
	{
		g_vertex[i].diffuse = color;
	}
}

//=======================================================
//�@�`�@��@���@��
//=======================================================
void DrawFade()
{
	g_pDevice = GetDevice();
	// ���_�����ݒ�
	g_pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	g_pDevice->SetTexture(0, NULL);

	// �|���S���`��(���_�z��g�p)
	g_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_vertex, sizeof(g_vertex[0]));
}

//=======================================================
//�@�t�F�[�h�C���J�n
//=======================================================
void  StartFadeIn()
{
	g_fadeCount = FADE_TIME;
	g_nDir = -1;
}
//=======================================================
//�@�t�F�[�h�A�E�g�J�n
//=======================================================
void StartFadeOut()
{
	g_fadeCount = 0;
	g_nDir = 1;
}

//=======================================================
//�@�t�F�[�h�C���I��
//=======================================================
int EndFadeIn()
{
	return (g_nDir < 0 && g_fadeCount <= 0);
}

//=======================================================
//�@�t�F�[�h�A�E�g�I��
//=======================================================
int EndFadeOut()
{
	return (g_nDir > 0 && g_fadeCount >= FADE_TIME);
}
