//---------------------------------------------------------
// エフェクト
// Effekseerを使用
//---------------------------------------------------------
#include "effect.h"
#include "camera.h"

::EffekseerRendererDX9::Renderer*	CEffect::m_renderer = NULL;
std::vector<CEffect*>	CEffect::m_vecEffect;
::EffekseerSound::Sound*		CEffect::m_sound = NULL;
::Effekseer::Manager*			CEffect::m_manager = NULL;
std::vector<::Effekseer::Effect*>			CEffect::m_effect;
IXAudio2*						CEffect::m_xa2 = NULL;
IXAudio2MasteringVoice*			CEffect::m_xa2_master = NULL;
//===========静的メンバ==============
void CEffect::LoadEffect(void) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 描画管理インスタンスの生成
	if (!m_renderer)
		m_renderer = ::EffekseerRendererDX9::Renderer::Create(pDevice, 1000);

	// XAudio2の初期化を行う
	if (!m_xa2)
		XAudio2Create(&m_xa2);
	if (!m_xa2_master)
		m_xa2->CreateMasteringVoice(&m_xa2_master);
	// サウンド管理インスタンスの生成
	if (!m_sound)
		m_sound = ::EffekseerSound::Sound::Create(m_xa2, 16, 16);

	if (!m_manager) {
		// エフェクト管理用インスタンスの生成
		m_manager = ::Effekseer::Manager::Create(1000);
		// 描画用インスタンスから描画機能を設定
		m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
		m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
		m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
		m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
		m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
		m_manager->SetModelLoader(m_renderer->CreateModelLoader());

		// 音再生用インスタンスから再生機能を指定
		m_manager->SetSoundPlayer(m_sound->CreateSoundPlayer());

		// 音再生用インスタンスからサウンドデータの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		m_manager->SetSoundLoader(m_sound->CreateSoundLoader());
	}

	// 投影行列を設定
	m_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(VIEW_ANGLE, CCamera::GetAspect(0), VIEW_NEAR_Z, VIEW_FAR_Z));

	//	m_manager->CreateCullingWorld(1000, 1000, 1000, 5);
		// エフェクトの読込
	if (!m_effect.size()) {
		::Effekseer::Effect* effect = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)EFFECT_PATH_SKILL1);
		m_effect.push_back(effect);
		effect = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)EFFECT_PATH_SKILL2);
		m_effect.push_back(effect);
		effect = NULL;
	}
}

void CEffect::CreateEffect(EFFECT effect, D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	CEffect* temp = new CEffect(effect, pos, rot);
	m_vecEffect.push_back(temp);
}

void CEffect::Uninit() {
	//if (m_effect.size()) {
	//	for (int i = 0; i < m_effect.size(); i++) {
			//SAFE_DELETE(&m_effect[i]);
			//delete &m_effect[i];
	//		int a=m_effect[i]->GetRef();
	//		m_effect[i]->UnloadResources();
	//		m_effect[i]->Release();
	//		m_effect[i] = NULL;
	//	}
	//	m_effect.clear();
	//}
	if (m_vecEffect.size()) {
		for (int i = 0; i < m_vecEffect.size(); i++) {
			SAFE_DELETE(m_vecEffect[i]);
		}
		m_vecEffect.clear();
	}
	// 描画用インスタンスを破棄
	//if (m_renderer) {
	//	m_renderer->Destroy();
		//m_renderer->Release();
		//delete &m_renderer;
	//	m_renderer = NULL;			
	//}
	// エフェクト管理用インスタンスを破棄
	//m_manager->Release();
	//m_manager->Destroy();
	//delete &m_manager;
	//m_manager = NULL;
	// サウンド用インスタンスを破棄
	//m_sound->Destroy();
	//delete &m_sound;
	//m_sound = NULL;
}
void CEffect::Update() {
	// 再生中なのでアップデート
	m_manager->Update();
	for (int i = 0; i < m_vecEffect.size(); i++) {
		// 再生終了していたらvectorから削除
		if (m_vecEffect[i]->CheckEffect()) {
			SAFE_DELETE(m_vecEffect[i]);
			m_vecEffect.erase(m_vecEffect.begin() + i);
			i--;
		}
	}
}
void CEffect::Draw(int camNo) {
	if (!m_vecEffect.size())	return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 camPos = CCamera::GetPosCamera(camNo);
	D3DXVECTOR3 lookatPos = CCamera::GetLookatPosCamera(camNo);
	// カメラ行列を設定
	CEffect::m_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(camPos.x, camPos.y, camPos.z), ::Effekseer::Vector3D(lookatPos.x, lookatPos.y, lookatPos.z), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	CEffect::m_renderer->BeginRendering();
	m_manager->Draw();
	CEffect::m_renderer->EndRendering();
}

//============メンバ関数==============

CEffect::CEffect(EFFECT effect, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// エフェクトの再生
	m_handle = m_manager->Play(m_effect[effect], pos.x, pos.y, pos.z);
	m_manager->SetRotation(m_handle, rot.x, -rot.y, rot.z);
	m_manager->SetScale(m_handle, 5, 5, 5);
}

CEffect::~CEffect()
{

}

bool CEffect::CheckEffect(void) {
	if (!m_manager->Exists(m_handle)) {
		return true;
	}
	return false;
}