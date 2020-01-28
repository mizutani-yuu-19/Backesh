//---------------------------------------------------------
// エフェクト
// Effekseerを使用
//---------------------------------------------------------
#pragma once

//---------インクルード--------
#include "main.h"
#include "game.h"

#include "effekseer/Effekseer.h"
#include "effekseer/EffekseerRendererDX9.h"
#include "effekseer/EffekseerSoundXAudio2.h"

#if _DEBUG
#pragma comment(lib, "effekseer/VS2015/Debug/Effekseer.lib" )
#pragma comment(lib, "effekseer/VS2015/Debug/EffekseerRendererDX9.lib" )
#pragma comment(lib, "effekseer/VS2015/Debug/EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "effekseer/VS2015/Release/Effekseer.lib" )
#pragma comment(lib, "effekseer/VS2015/Release/EffekseerRendererDX9.lib" )
#pragma comment(lib, "effekseer/VS2015/Release/EffekseerSoundXAudio2.lib" )
#endif


//-----------列挙体----------------
typedef enum {
	EFFECT_SKILL1,
	EFFECT_SKILL2,
	EFFECT_MAX
}EFFECT;
//-----------定数----------------
#define EFFECT_PATH_SKILL1	(L"data/EFFECT/skill1.efk")
#define EFFECT_PATH_SKILL2	(L"data/EFFECT/skill2.efk")

class CEffect
{
public:
	CEffect(EFFECT effect,D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	~CEffect();

	static void CreateEffect(EFFECT effect,D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static void Uninit();
	static void Update();
	static void Draw(int camNo);
	static void LoadEffect(void);
	static ::EffekseerRendererDX9::Renderer*	m_renderer;

private:
	::Effekseer::Handle				m_handle = -1;
	bool CheckEffect(void);

	static ::Effekseer::Manager*			m_manager;
	static ::EffekseerSound::Sound*		m_sound;
	static std::vector<CEffect*>	m_vecEffect;
	static std::vector<::Effekseer::Effect*>		m_effect;
	static IXAudio2*						m_xa2;
	static IXAudio2MasteringVoice*			m_xa2_master;
};
