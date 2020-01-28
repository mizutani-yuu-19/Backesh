///////////////////////////////////////////////////
//
// �L�����N�^�[
//
//////////////////////////////////////////////////
//----------------�C���N���[�h--------------
#include "character.h"
#include "game.h"
#include "game_pad.h"
#include "camera.h"
#include "collision.h"
#include "input.h"
#include "water_puddle.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "effect.h"

//-------------�萔�}�N��--------------
#define	VALUE_MOVE_SPEED	(5.0f)					// �ړ����x
#define	VALUE_AVOID_SPEED	(10.0f)					// ��𑬓x
#define AVOID_COOL_TIME		(1.0f * 60)						// ����N�[���^�C��1�b
#define AVOID_INVINCIBLE_TIME		(0.5f * 60)				// ��𖳓G����
#define START_INVINCIBLE_TIME		(3.0f * 60)				// �X�^�[�g�����G����

#define SKILL_TIME			(2.0f*60.0f)			// �X�L���g�p����
//----------�O���[�o��-----------

//---------------�����o�֐�---------------

// �R���X�g���N�^
CPlayer::CPlayer(int characterType, int teamNo, int bucketType) :CCharacter(characterType, teamNo, bucketType)
{

}

// �f�X�g���N�^
CPlayer::~CPlayer() {

}

// �ʒu�Ƃ�������
void CPlayer::Init() {
	m_lockOnNo = 9999;
	CCharacter::Init();
}

void CPlayer::Uninit() {

}

void CPlayer::Update() {
	//�e�L�����N�^�[�̓����̐ݒ�
	if (CGame::GetStep() == STEP_GAME) {
		if (!m_skillFlag&&m_surviveFlag&&m_pickUpTime <= 0) {
			Operate();
		}
	}
	// ���b�N�I���X�V
	LockOnUpdate();
	SkillUpdate();
	// �L�����N�^�[�̃A�b�v�f�[�g
	CCharacter::Update();

}

void CPlayer::Draw(int camNo) {
	PrintDebugProc("\nX:%f Y:%f Z:%f\n", m_afterPos.x, m_afterPos.y, m_afterPos.z);
	CCharacter::Draw(camNo);
	if (this->m_teamNo == camNo)
		LockOnDraw();
}


// ����
void CPlayer::Operate()
{
	// �R���g���[������
	D3DXMATRIX *y = new D3DXMATRIX;
	// �J�����̌�������
	m_rot.y += GetPadRIGHTStickX(m_teamNo)*-0.1f;
	if (DIGetKey(DIK_RIGHTARROW)) {
		m_rot.y -= 0.1f;
	}
	if (DIGetKey(DIK_LEFTARROW)) {
		m_rot.y += 0.1f;
	}
	// �ړ�����
	m_move.x = GetPadLEFTStickX(m_teamNo);
	m_move.z = GetPadLEFTStickY(m_teamNo);
	if (m_move.x * m_move.x + m_move.z * m_move.z > 0.000001f)  //�������O�ɋ߂��Ȃ���Ύ΂ߕ␳���s��
	{
		float a = (fabs(m_move.x) >= fabs(m_move.z)) ? m_move.z / m_move.x : m_move.x / m_move.z;       //��Βl�̏���������傫�����Ŋ���
		float b = 1.0f / sqrt(1.0f + a * a);        //�␳��

		m_move.x *= b * m_moveSpeed;
		m_move.z *= b * m_moveSpeed;
	}
#ifdef _DEBUG
	if (DIGetKey(DIK_W)) {
		m_move.z = m_moveSpeed;
	}
	if (DIGetKey(DIK_S)) {
		m_move.z = -1.0f*m_moveSpeed;
	}
	if (DIGetKey(DIK_D)) {
		m_move.x = m_moveSpeed;
	}
	if (DIGetKey(DIK_A)) {
		m_move.x = -1.0f*m_moveSpeed;
	}
#endif // _DEBUG

	if (fabs(m_move.x) > fabs(m_move.z)*2.0f) {
		m_sideStepFlag = true;
	}
	else {
		m_sideStepFlag = false;
	}
	// Y����]
	D3DXVec3TransformCoord(&m_move, &m_move, D3DXMatrixRotationY(y, -1.0f * m_rot.y));

	// ���擾
#ifdef _DEBUG
	if ((GetPadButton(m_teamNo, PAD_FLAG_PRESS, PAD_X) || DIGetKey(DIK_RSHIFT))) {
#else
	if ((GetPadButton(m_teamNo, PAD_FLAG_PRESS, PAD_X))) {
#endif // _DEBUG
		PickUpWater();
	}

#ifdef _DEBUG
	if (GetPadRIGHTTrigger(m_teamNo) || DIGetKeyTrigger(DIK_RETURN))
#else
	if (GetPadRIGHTTrigger(m_teamNo))
#endif // _DEBUG
	{// �e�̔���
	 // �U�����łȂ�
		if (!m_water&&m_waterVolume > 0.0f) {
			D3DXVECTOR3 pos;
			pos = m_afterPos;
			pos.y += 10.0f;
			m_water = new CWaterAtk(pos, m_rot.y*-1.0f, m_waterVolume, m_bucketType);
			m_waterVolume = 0.0f;

			m_mesh.SetAnimSpeed(2.0f);
			m_mesh.SetTime(0.1f);
			m_mesh.SwitchAnimSet(ANIM_SPLASH, 1.0f);
			PlaySound(SOUND_LABEL_SPLASH1 + m_teamNo*MAX_TEAMMEMBER + m_No, 0.7f);
		}
	}

#ifdef _DEBUG
	if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_B) || DIGetKeyTrigger(DIK_TAB))
#else
	if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_B))
#endif // _DEBUG
	{// �K�E�Z
		if (m_skillGauge >= 1.0f) {
			D3DXVECTOR3 pos = m_beforePos;
			pos.y += 10;
			CEffect::CreateEffect((EFFECT)m_bucketType, pos, m_rot);
			m_skillFlag = true;
			m_skillTime = 0;
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

#ifdef _DEBUG
	if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_SPACE))
#else
	if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_A))
#endif // _DEBUG
	{// �W�����v
		if (!m_jumpFlag) {
			m_move += D3DXVECTOR3(0.0f, 5.0f, 0.0f);
			m_jumpFlag = true;
			m_mesh.SwitchAnimSet(ANIM_JUMP, 0.5f);
			m_mesh.SetTime(0.5f);
		}
	}
#ifdef _DEBUG
	if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_RIGHT_BUTTON) || DIGetKeyTrigger(DIK_E)) {
#else
	// �v���C���[�`�F���W
	if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_RIGHT_BUTTON)) {
#endif // _DEBUG
		changeFrom = this;
		m_lockOnNo = 9999;
		changeTo = m_character[m_teamNo][(m_No + 1) % MAX_TEAMMEMBER];
	}
#ifdef _DEBUG
	else if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_LEFT_BUTTON) || DIGetKeyTrigger(DIK_Q)) {
#else
	// �v���C���[�`�F���W
	else if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_LEFT_BUTTON)) {
#endif // _DEBUG
		changeFrom = this;
		m_lockOnNo = 9999;
		changeTo = m_character[m_teamNo][(m_No + MAX_TEAMMEMBER - 1) % MAX_TEAMMEMBER];
	}
#ifdef _DEBUG
	if ((GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_Y) || DIGetKeyTrigger(DIK_X)) && m_avoidCoolTime <= 0.0f) {
#else
	// ���
	if ((GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_Y)) && m_avoidCoolTime <= 0.0f) {
#endif // _DEBUG
		m_avoidCoolTime = AVOID_COOL_TIME;	// �N�[���^�C���ݒ�
		m_invincibleTime = AVOID_INVINCIBLE_TIME;	// ��𖳓G���Ԑݒ�
		m_moveSpeed = VALUE_AVOID_SPEED;			// ����X�s�[�h	
													// �ړ�����
		m_move.x = GetPadLEFTStickX(m_teamNo) * m_moveSpeed;
		m_move.z = GetPadLEFTStickY(m_teamNo) * m_moveSpeed;
#ifdef _DEBUG
		if (DIGetKey(DIK_W)) {
			m_move.z = m_moveSpeed;
		}
		if (DIGetKey(DIK_S)) {
			m_move.z = -1.0f*m_moveSpeed;
		}
		if (DIGetKey(DIK_D)) {
			m_move.x = m_moveSpeed;
		}
		if (DIGetKey(DIK_A)) {
			m_move.x = -1.0f*m_moveSpeed;
		}
#endif // _DEBUG
		// Y����]
		D3DXVec3TransformCoord(&m_move, &m_move, D3DXMatrixRotationY(y, -1.0f * m_rot.y));
	}
	else if (m_invincibleTime <= 0.0f && m_moveSpeed != m_maxMoveSpeed) {
		m_moveSpeed = m_maxMoveSpeed;
	}
#ifdef _DEBUG
	if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_LEFT_TRIGGER) || DIGetKeyTrigger(DIK_F)) {
#else
	// ���b�N�I��
	if (GetPadButton(m_teamNo, PAD_FLAG_TRIGGER, PAD_LEFT_TRIGGER)) {
#endif // _DEBUG
		LockOn();
	}
	SAFE_DELETE(y);
}

// ���b�N�I��
void CPlayer::LockOn() {
	if (m_lockOnNo == 9999) {
		float dist = 9999;
		for (int teamNo = 0; teamNo < m_teamNum; teamNo++) {
			if (m_teamNo == teamNo)	continue;		// �����`�[���i���o�[�Ȃ�
			for (int charaNo = 0; charaNo < MAX_TEAMMEMBER; charaNo++) {
				D3DXVECTOR3 pos = m_beforePos - GetCharacterPos(teamNo, charaNo);
				if (dist < D3DXVec3Length(&pos))	continue;
				dist = D3DXVec3Length(&pos);
				if (dist > 400)		continue;
				// �p�x�����߂�
				D3DXVECTOR3 vec1 = D3DXVECTOR3(0, 0, 0);
				D3DXVECTOR3 vec2 = D3DXVECTOR3(0, 0, 0);
				vec1.x = sin(m_rot.y);
				vec1.z = -cos(m_rot.y);
				D3DXVec3Normalize(&vec1, &vec1);
				vec2 = m_beforePos - GetCharacterPos(teamNo, charaNo);
				D3DXVec3Normalize(&vec2, &vec2);
				float enemyRad = acosf(D3DXVec3Dot(&vec1, &vec2));
				if (enemyRad > D3DX_PI*0.3f)	continue;
				// �p�x���͈͓��Ȃ�
				m_lockOnNo = teamNo * 10 + charaNo;
			}
		}
	}
	else {
		m_lockOnNo = 9999;
	}
}

// ���b�N�I�����A�b�v�f�[�g
void CPlayer::LockOnUpdate() {
	// ���������񂾂�
	if (!m_surviveFlag) {
		m_lockOnNo = 9999;
		return;
	}
	// ���b�N�I����
	if (m_lockOnNo != 9999) {
		m_lockOnPos = GetCharacterPos(m_lockOnNo / 10, m_lockOnNo % 10);
		D3DXVECTOR3 vec1 = m_lockOnPos - m_beforePos;
		if (D3DXVec3Length(&vec1) > 400) {
			m_lockOnNo = 9999;
			return;
		}
		else if (!GetCharacterSurvive(m_lockOnNo / 10, m_lockOnNo % 10)) {
			m_lockOnNo = 9999;
			return;
		}
		else {
			D3DXVec3Normalize(&vec1, &vec1);
			m_rot.y = atan2(vec1.z, vec1.x) - D3DX_PI*0.5f;
			return;
		}
	}
}
// ���b�N�I���`��
void CPlayer::LockOnDraw() {
	if (m_lockOnNo != 9999) {
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXMATRIX mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		struct COLOR_VERTEX {
			D3DXVECTOR3 vecCoord;
			//	FLOAT w;
			FLOAT fScale;
			DWORD dwColor;
		};

		int dispx = GetSystemMetrics(SM_CXSCREEN);
		if (CGame::GetGameParam().playerNum > 2) {
			dispx *= 0.5f;
		}
		FLOAT fScale = dispx*0.1f;

		m_lockOnPos.y += 10;

		COLOR_VERTEX lockOn = { m_lockOnPos,fScale,D3DCOLOR_ARGB(255,255,255,255) };

		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		// �[�x�o�b�t�@������
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		// ���C�e�B���O������
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		// �|�C���g�X�v���C�g�L����
		pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);

		pDevice->SetTexture(0, m_pD3DTextureLockOn);
		pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE);
		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_POINTLIST, 1, &lockOn, sizeof(COLOR_VERTEX));

		// ���C�e�B���O�L����
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}

// �K�E�Z�X�V
void CPlayer::SkillUpdate() {
	// �Q�[�W���^����
	if (m_skillFlag) {
		// �X�L���g�p��
		if (m_skillTime < SKILL_TIME) {
			m_skillTime++;
			m_skillGauge -= 0.01f;
			switch (m_bucketType)
			{
			case 0:
				// �c
				if (m_skillTime < 20) break;
				for (int i = 0; i < m_teamNum; i++) {
					if (i == m_teamNo)	continue;	// ���`�[��
					for (int j = 0; j < MAX_TEAMMEMBER; j++) {
						if (!GetCharacterSurvive(i, j))	continue; // ����ł���
						if (!GetCharacterInvincible(i, j))	continue; // ���G�Ȃ�
						static int height = 1000;
						static int width = 150;
						if (GetCharacterPos(i, j).y - m_beforePos.y > width*0.5f || GetCharacterPos(i, j).y - m_beforePos.y < -width*0.5f)	continue;
						D3DXVECTOR2 enemyPos = D3DXVECTOR2(GetCharacterPos(i, j).x, GetCharacterPos(i, j).z);
						D3DXVECTOR2 point[4];
						point[0] = D3DXVECTOR2(-width*0.5f, +0);
						point[1] = D3DXVECTOR2(+width*0.5f, +0);
						point[2] = D3DXVECTOR2(+width*0.5f, +height);
						point[3] = D3DXVECTOR2(-width*0.5f, +height);

						D3DXMATRIX matrixY;
						D3DXMatrixRotationZ(&matrixY, m_rot.y);

						D3DXVec2TransformCoord(&point[0], &point[0], &matrixY);
						D3DXVec2TransformCoord(&point[1], &point[1], &matrixY);
						D3DXVec2TransformCoord(&point[2], &point[2], &matrixY);
						D3DXVec2TransformCoord(&point[3], &point[3], &matrixY);

						for (int k = 0; k < 4; k++) {
							point[k] += D3DXVECTOR2(m_beforePos.x, m_beforePos.z);
						}

						D3DXVECTOR2 vec1[4];
						D3DXVECTOR2 vec2[4];
						for (int k = 0; k < 4; k++) {
							vec1[k] = enemyPos - point[k];
							vec2[k] = point[(k + 1) % 4] - point[k];
							if (fabs(vec1[k].x) < 0.001f)	vec1[k].x = 0;
							if (fabs(vec1[k].y) < 0.001f)	vec1[k].y = 0;
							if (fabs(vec2[k].x) < 0.001f)	vec2[k].x = 0;
							if (fabs(vec2[k].y) < 0.001f)	vec2[k].y = 0;
						}

						for (int k = 0; k < 4; k++) {
							float cross = vec1[k].x*vec2[k].y - vec2[k].x*vec1[k].y;
							if (cross > 0) {
								break;
							}
							if (k == 3) {
								CharacterKill(i, j, m_teamNo * 10 + m_No);
							}
						}
					}
				}
				break;
			case 1:
				if (m_skillTime < 20) break;
				// ��
				for (int i = 0; i < m_teamNum; i++) {
					if (i == m_teamNo)	continue;	// ���`�[��
					for (int j = 0; j < MAX_TEAMMEMBER; j++) {
						if (!GetCharacterSurvive(i, j))	continue; // ����ł���
						if (!GetCharacterInvincible(i, j))	continue; // ���G�Ȃ�
						D3DXVECTOR3 enemyPos = GetCharacterPos(i, j);
						enemyPos -= m_beforePos;
						D3DXVECTOR2 enepos = D3DXVECTOR2(enemyPos.x, enemyPos.z);
						float length = D3DXVec2Length(&enepos);
						if (length < 250) {
							CharacterKill(i, j, m_teamNo * 10 + m_No);
						}
					}
				}
				break;
			default:
				break;
			}
		}
		else {
			// �X�L���I��
			m_skillFlag = false;
			m_skillGauge = 0.0f;
			m_skillTime = 0.0f;
		}
	}
}