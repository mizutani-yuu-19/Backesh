//=============================================================================
//
// �����܂菈�� [water_puddle.h]
//
//=============================================================================
#ifndef _WATER_PUDDLE_H_
#define _WATER_PUDDLE_H_

#include "main.h"
//-----------�萔�}�N��----------
#define PUDDLE_SIZE				(2.0f)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CWaterPuddle
{
public:
	CWaterPuddle(D3DXVECTOR3 pos, float vol, bool infinite);
	~CWaterPuddle();

	void Init(void);			//����������
	void Uninit(void);			//�I������
	void Update(void);					//�X�V����
	void Draw(void);					//�`�揈��
	D3DXVECTOR3			m_pos;					// �ʒu
	D3DXVECTOR3			m_size;					// �傫��(���f���T�C�Y�Ɗ|�����킹��)
	static D3DXVECTOR3	GetWaterPuddlePos(int no);	//�����܂�̍��W���擾
	static D3DXVECTOR3	GetWaterPuddleSize(int no);	//�����܂�̑傫�����擾
	static int			GetWaterHandle(int no);			// �����܂�̃n���h���擾
	static float		GetWaterPuddleVolume(int no);	//�����܂�̐���

	static void CreateWaterPuddle(D3DXVECTOR3 pos, float vol, bool infinite);	// �����܂萶��
	static void LoadModel();											// �����܂�̃��f�����[�h
	static void ReleaseModel();											// �����܂�̃��f���J��
	static void UpdateWaterPuddle();									// �����܂�̑S�X�V
	static void DrawWaterPuddle();										// �����܂�̑S�`��
	static void SubRemain(int no);										// ���ʂ����炷�i�w��j	
	static void SubRemain2(int handle, float volume);									// ���ʂ����炷�i�w��j	
	void Absorb();														// ���ʂ����炷
	void Absorb(float vol);														// ���ʂ����炷

	static std::vector<CWaterPuddle*>	m_waterPuddle;

	static int			NoCnt;					// ��ӂȔԍ�
	int					m_handle;					// ���̖�			�ꎞ�I��public��
private:
	D3DXMATRIX			m_mtxWorld;				// ���[���h�}�g���N�X
	float				m_remain;				// �c�萅��
	bool				m_infiniteFlag;			// �����ɐ�������t���O
	static D3DXVECTOR3			m_modelSize;
};


#endif
