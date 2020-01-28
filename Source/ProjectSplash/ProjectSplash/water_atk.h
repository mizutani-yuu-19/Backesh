//=============================================================================
//
// ���f������ [water_atk.h]
//
//=============================================================================
#ifndef _WATERATK_H_
#define _WATERATK_H_

#include "main.h"


//---------�萔�}�N��------------
#define WATER_SIZE				(2.0f)
#define WATER_MAX_COL			(750)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 move;			//���x
	D3DCOLOR	color;			//�F
	D3DXVECTOR3 accel;			//�����x
	int			count;			//��������
	bool		bUse;			//�g�p���Ă��邩�ǂ���
	int			delay;			// ��������������΂�
	int			colGroup;		// �����蔻��O���[�v
}WATER_PARTICLE;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CWaterAtk
{
public:
	CWaterAtk();
	CWaterAtk(D3DXVECTOR3 pos, float rot, float pow, int pat);
	~CWaterAtk();

	void Init(void);			//����������
	void Uninit(void);			//�I������
	bool Update(void);			//�X�V����
	void Draw(int no);			//�`�揈��
	D3DXVECTOR3 GetPos(int no,int *num);				// �����蔻��no�O���[�v�̃p�[�e�B�N���̈ʒu�Ƃ��̃O���[�v�Ɋ܂܂��Փˌ���Ԃ�
	int GetNumParticle();								// ���ː��擾
	static void CreateBillboard();						// �r���{�[�h�쐬�ƃe�N�X�`���ǂݍ���
	static void ReleaseBillboard();						// �r���{�[�h�ƃe�N�X�`���j��
	void SetUseDown(int no);								// use�t���O�����낷
private:
	int m_delay;										// ��C�ɏo�Ȃ��悤��
	int m_numParticle;
	bool m_puddleFlag[3];										// ���ɐ������t���O
	D3DXVECTOR3	m_puddlePos[3];
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	int m_pat;						// �c�����A������
	WATER_PARTICLE *m_particle;		// �p�[�e�B�N�����
	WATER_PARTICLE*	m_firstParticle;		// �p�[�e�B�N���擪�A�h���X
	std::vector<WATER_PARTICLE*> m_colList[WATER_MAX_COL];			// ���񂳂ꂽ�p�[�e�B�N��

	int cnt = 0;
	int particleNo[1000];

};

#endif
