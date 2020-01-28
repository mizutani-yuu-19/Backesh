//---------------------------------------------------------
// �G�t�F�N�g
// Effekseer���g�p
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
//===========�ÓI�����o==============
void CEffect::LoadEffect(void) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �`��Ǘ��C���X�^���X�̐���
	if (!m_renderer)
		m_renderer = ::EffekseerRendererDX9::Renderer::Create(pDevice, 1000);

	// XAudio2�̏��������s��
	if (!m_xa2)
		XAudio2Create(&m_xa2);
	if (!m_xa2_master)
		m_xa2->CreateMasteringVoice(&m_xa2_master);
	// �T�E���h�Ǘ��C���X�^���X�̐���
	if (!m_sound)
		m_sound = ::EffekseerSound::Sound::Create(m_xa2, 16, 16);

	if (!m_manager) {
		// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
		m_manager = ::Effekseer::Manager::Create(1000);
		// �`��p�C���X�^���X����`��@�\��ݒ�
		m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
		m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
		m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
		m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
		m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

		// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
		m_manager->SetModelLoader(m_renderer->CreateModelLoader());

		// ���Đ��p�C���X�^���X����Đ��@�\���w��
		m_manager->SetSoundPlayer(m_sound->CreateSoundPlayer());

		// ���Đ��p�C���X�^���X����T�E���h�f�[�^�̓Ǎ��@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		m_manager->SetSoundLoader(m_sound->CreateSoundLoader());
	}

	// ���e�s���ݒ�
	m_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(VIEW_ANGLE, CCamera::GetAspect(0), VIEW_NEAR_Z, VIEW_FAR_Z));

	//	m_manager->CreateCullingWorld(1000, 1000, 1000, 5);
		// �G�t�F�N�g�̓Ǎ�
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
	// �`��p�C���X�^���X��j��
	//if (m_renderer) {
	//	m_renderer->Destroy();
		//m_renderer->Release();
		//delete &m_renderer;
	//	m_renderer = NULL;			
	//}
	// �G�t�F�N�g�Ǘ��p�C���X�^���X��j��
	//m_manager->Release();
	//m_manager->Destroy();
	//delete &m_manager;
	//m_manager = NULL;
	// �T�E���h�p�C���X�^���X��j��
	//m_sound->Destroy();
	//delete &m_sound;
	//m_sound = NULL;
}
void CEffect::Update() {
	// �Đ����Ȃ̂ŃA�b�v�f�[�g
	m_manager->Update();
	for (int i = 0; i < m_vecEffect.size(); i++) {
		// �Đ��I�����Ă�����vector����폜
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
	// �J�����s���ݒ�
	CEffect::m_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(camPos.x, camPos.y, camPos.z), ::Effekseer::Vector3D(lookatPos.x, lookatPos.y, lookatPos.z), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	CEffect::m_renderer->BeginRendering();
	m_manager->Draw();
	CEffect::m_renderer->EndRendering();
}

//============�����o�֐�==============

CEffect::CEffect(EFFECT effect, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �G�t�F�N�g�̍Đ�
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